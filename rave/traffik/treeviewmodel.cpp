#include <fstream>

#include <QStandardItem>
#include <QDebug>

#include "treeviewmodel.h"
#include "tree.h"

TreeViewModel::TreeViewModel(QList<QTableWidgetItem*> items, QObject *parent)
    :QStandardItemModel{ parent }
{
    setColumnCount(1);
    m_root_item = invisibleRootItem();
    read_tree_data(items);
    build_tree(m_nodes);
    //print_tree(m_tree, 0);
}

int TreeViewModel::read_tree_data(const SelectedItems& items)
{
    enum Columns{Date_Column, Time_Column, Duration_Column};

    int _node_id = -1;
    int _parent_id = -1;
    int _prev_pid = -1;

    std::string root_name{};
    std::string node_name{};

    struct Row{
        int node_id{-1};
        std::string root_name{""};
        int parent_id{-1};
        std::string node_name{""};
    };

    std::vector<Row> rows;

    for (int i=0; i<items.count(); ++i){

        auto item = items.at(i);

        switch (item->column()){
            case Date_Column:
            {
                _node_id = item->data(Qt::UserRole).toInt();
                root_name = item->text().toStdString();
                break;
            }
            case Time_Column:
            {
                _parent_id = item->data(Qt::UserRole).toInt();
                node_name = item->text().toStdString();
                break;
            }
            case Duration_Column:
                continue;
        }

        if (_node_id > -1 && _parent_id > -1){
            Row row;
            row.node_id = _node_id;
            row.root_name = root_name;
            row.parent_id = _parent_id;
            row.node_name = node_name;

            rows.push_back(row);

            _node_id = -1;
            _parent_id = -1;
        }
    }

    std::sort(rows.begin(), rows.end(),
            [](const Row lhs,  const Row rhs){ return (lhs.parent_id < rhs.parent_id); });

    for (auto& row : rows){
        // create root node
        Node* node;
        if (_prev_pid == row.parent_id){
            // Make leaf
            node = new Node(row.node_name, row.root_name, row.node_id, row.parent_id);
            node->setData(row.node_id, Qt::UserRole);
            m_nodes.push_back(node);
        } else{
            // Make header
            Node* node_root = new Node(row.root_name, row.node_name,  row.parent_id, -1);
            node_root->setData(row.node_id, Qt::UserRole);
            m_nodes.push_back(node_root);

            // ... then make a leaf
            Node* node_leaf = new Node(row.node_name, row.root_name, row.node_id, row.parent_id);
            node_leaf->setData(row.node_id, Qt::UserRole);
            m_nodes.push_back(node_leaf);

            _prev_pid = row.parent_id;
        }

    }

 return 1;

}

void TreeViewModel::build_tree(std::vector<Node *> &nodes)
{
    for (auto node : nodes){
        if (node->parent_id() == -1){
            m_root_item->appendRow(node);
            m_tree[node->node_id()] = node;
        }else {
            insert_node(m_tree, node);
        }
    }

}

void TreeViewModel::treeClicked(QModelIndex mindex)
{
    qDebug() << "C++ " << mindex.data(Qt::UserRole).toInt();

}


void TreeViewModel::insert_node(TreeNode tree_node, Node* node)
{
    bool found=false;
    for(auto& [id, parent] : tree_node){
        if (found) return;
        if (id == node->parent_id()){
            parent->children()[node->node_id()] = node;
            parent->appendRow(node);
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
}


void TreeViewModel::print_tree(TreeNode tree_node, int level)
{
    QString indent{""};
    for(int i=0; i<level; ++i)
        indent +="  ";

    for(auto& [id, node] : tree_node){
        //std::cout << indent << "|-" << id <<".:"<< node->name << "(" << node->children.size() << ")" << '\n';
        //qDebug() << indent << "|-" << id << ".:" << QString::fromStdString(node->node_name());
        //print_tree(node->children(), ++level);
        qDebug() << QString::fromStdString(node->node_name());
    }

}


TreeData TreeViewModel::tokenize(std::string line)
{
     size_t pos = 0;
     std::string token;
     std::string delimeter = "|";

     std::vector<std::string> tokens;

     while((pos = line.find(delimeter)) != std::string::npos){
         token = line.substr(0, pos);
         tokens.push_back(token);
         line.erase(0, pos + delimeter.length());
     }

     TreeData tok_tuple = std::make_tuple(tokens.at(0), tokens.at(1), tokens.at(2));

     return tok_tuple;
}



