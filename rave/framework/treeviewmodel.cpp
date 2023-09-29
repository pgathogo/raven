#include <iostream>
#include <fstream>

#include <QStandardItem>
#include <QDebug>

#include "treeviewmodel.h"

#include "../utils/tools.h"

TreeViewModel::TreeViewModel(std::vector<NodeData*> node_data,  QObject* parent)
    :QStandardItemModel{ parent }
    ,m_node_data{node_data}
{
    setColumnCount(1);
    rootItem = invisibleRootItem();

    build_tree_nodes(node_data);
    build_tree(nodes);
}

TreeViewModel::~TreeViewModel()
{
}

void TreeViewModel::update_model(NodeData* node_data)
{
    m_node_data.push_back(node_data);
}

void TreeViewModel::build_tree_nodes(std::vector<NodeData*>& node_data)
{
    for (auto data : node_data){
        Node* node = new Node(data->name, data->id, data->parent_id);
        node->setData(data->id, Qt::UserRole);
        nodes.push_back(node);
    }
    // sort by parent id
    std::sort(nodes.begin(), nodes.end(),
            [](const Node* lhs,  const Node* rhs){ return (lhs->parentID() < rhs->parentID()); });
}

void TreeViewModel::build_tree(std::vector<Node *> &nodes)
{
    for (auto node : nodes){
        if (node->parentID() == 0){
            node->setIcon(QIcon(":/rave_images/media/icons/main_folder_sm.bmp"));
            rootItem->appendRow(node);
            tree[node->nodeID()] = node;
        }else {
            node->setIcon(QIcon(":/rave_images/media/icons/audiofolder_sm.bmp"));
            insert_node(tree, node);
        }
    }

}

void TreeViewModel::insert_node(TreeNode tree_node, Node* node)
{
    bool found=false;
    for(auto& [id, parent] : tree_node){
        if (found) return;
        if (id == node->parentID()){
            parent->add_child(node);
            parent->appendRow(node);
            parent->setIcon(QIcon(":/rave_images/media/icons/main_folder_sm.bmp"));
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
}

std::vector<NodeData*> TreeViewModel::nodes_data()
{
    return m_node_data;
}

void TreeViewModel::rebuild_tree()
{
   // beginResetModel();
   // nodes.clear();
   // endResetModel();

    build_tree_nodes(m_node_data);
    build_tree(nodes);
}


void TreeViewModel::treeClicked(QModelIndex mindex)
{
    qDebug() << "C++ " << mindex.data(Qt::UserRole).toInt();

}

void TreeViewModel::print_tree(TreeNode tree_node, int level)
{
    //std::string indent = "";
    QString indent{""};
    for(int i=0; i<level; ++i)
        indent +="  ";

    for(auto& [id, node] : tree_node){
        //std::cout << indent << "|-" << id <<".:"<< node->name << "(" << node->children.size() << ")" << '\n';
        qDebug() << indent << "|-" << id << ".:" << QString::fromStdString(node->nodeName());
        print_tree(node->children(), ++level);
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

/*
QVariant TreeViewModel::data(const QModelIndex& index, int role) const
{
    if (!nodes.empty() && role == Qt::DisplayRole){
//        qDebug() << nodes.at(index.row())->text();
        return nodes.at(index.row())->text();
    }
    return QVariant();
}

QModelIndex TreeViewModel::index(int row, int col, const QModelIndex& parent) const
{
    if (hasIndex(row, col, parent))
        return createIndex(row, col);
    return QModelIndex();
}


int TreeViewModel::rowCount(const QModelIndex& parent) const
{
    return nodes.size();
}
*/
