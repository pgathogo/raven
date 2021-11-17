#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

#include <QDebug>

#include "tree.h"

Node::Node()
    :node_name{},
      node_id{-1},
      parent_id{-1}
{
}

Node::Node(std::string name, int n_id, int p_id)
    :node_name{name},
     node_id{n_id},
     parent_id{p_id}
{
    setText(QString::fromStdString(name));
}

Node::~Node(){}

Tree::Tree()
{
}

int Tree::read_tree_data()
{
    TreeNode tree;
    std::vector<Node*> nodes;
    std::string line;
    std::ifstream tree_file(tree_data_file);

    if (tree_file.is_open())
    {
        while (std::getline (tree_file, line))
        {
            auto[id, name, parent_id] = tokenize(line);
            int _id = std::stoi(id);
            int _parent = std::stoi(parent_id);
            Node* node = new Node(name, _id, _parent);
            nodes.push_back(node);
        }

        // sort by parent id
        std::sort(nodes.begin(), nodes.end(),
                [](const Node* lhs,  const Node* rhs){ return (lhs->parentID() < rhs->parentID()); });

        // Build tree
        for (auto node : nodes){
            if (node->parentID() == 0){
                tree[node->nodeID()] = node;
            }else {
                insert_node(tree, node);
            }
        }

    }else{
        qDebug() << "Unable to open file for reading!";
        return 0;
    }

    //print_tree(tree, 0);

 return 1;

}

void Tree::insert_node(TreeNode tree_node, Node* node)
{
    bool found=false;
    for(auto& [id, parent] : tree_node){
        if (found) return;
        if (id == node->parentID()){
            parent->children()[node->nodeID()] = node;
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
}


void Tree::print_tree(TreeNode tree_node, int level)
{
    std::string indent = "";
    for(int i=0; i<level; ++i)
        indent +="  ";

    for(auto& [id, node] : tree_node){
        //std::cout << indent << "|-" << id <<".:"<< node->name << "(" << node->children.size() << ")" << '\n';
        print_tree(node->children(), ++level);
    }

}


TreeData Tree::tokenize(std::string line)
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
