#ifndef TREE_H
#define TREE_H

#include <string>
#include <map>
#include <QStandardItem>

class Node;

using NodeID = int;

using TreeNode = std::map<NodeID, Node*>;

using TreeData = std::tuple<std::string, std::string, std::string>;

class Node : public QStandardItem{
public:
    Node();
    Node(std::string name, std::string parent_name, int n_id, int p_id);
    ~Node();

    std::string node_name() const { return m_node_name; }
    std::string parent_name() const { return m_parent_name; }
    int node_id() const { return m_node_id; }
    int parent_id() const { return m_parent_id; }
    TreeNode children(){ return m_children; }

private:
    std::string m_node_name;
    std::string m_parent_name;
    int m_node_id;
    int m_parent_id;
    TreeNode m_children;
};

/*
class Tree
{
public:
    //const std::string tree_data_file{"tree.txt"};

    Tree();
    int read_tree_data();
    void insert_node(TreeNode tree_node, Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
};
*/

#endif // TREE_H
