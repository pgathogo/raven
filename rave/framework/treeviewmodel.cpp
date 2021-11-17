#include <fstream>

#include <QStandardItem>
#include <QDebug>

#include "treeviewmodel.h"
#include "tree.h"

TreeViewModel::TreeViewModel(std::vector<NodeData*>& node_data,  QObject* parent)
    :QStandardItemModel{ parent }
{
    setColumnCount(1);
    rootItem = invisibleRootItem();
    build_tree_nodes(node_data);
    build_tree(nodes);
}

TreeViewModel::~TreeViewModel()
{
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
            rootItem->appendRow(node);
            tree[node->nodeID()] = node;
        }else {
            insert_node(tree, node);
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
        if (id == node->parentID()){
            parent->children()[node->nodeID()] = node;
            parent->appendRow(node);
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
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



