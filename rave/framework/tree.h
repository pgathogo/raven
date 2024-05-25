#ifndef TREE_H
#define TREE_H

#include <string>
#include <map>
#include <QStandardItem>


namespace FRAMEWORK {

class Node;
using NodeID = int;
using TreeNode = std::map<NodeID, Node*>;
using TreeData = std::tuple<std::string, std::string, std::string>;

    struct NodeData{
        NodeData();
        int id{ 0 };
        int parent_id { 0 };
        std::string name{""};
    };

    class Node : public QStandardItem{
    public:
        Node();
        Node(std::string name, int n_id, int p_id);
        ~Node();

        std::string nodeName() const { return node_name; }
        int nodeID() const { return node_id; }
        int parentID() const { return parent_id; }
        TreeNode children(){ return mChildren; }

        void add_child(Node*);

    private:
        std::string node_name;
        int node_id;
        int parent_id;
        TreeNode mChildren;
    };

    class Tree
    {
    public:
        const std::string tree_data_file{"tree.txt"};

        Tree();
        int read_tree_data();
        void insert_node(TreeNode tree_node, Node* node);
        void print_tree(TreeNode tree_node, int level=0);
        TreeData tokenize(std::string line);
    };

}
#endif // TREE_H
