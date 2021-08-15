#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>

class Node;
class NodeData;

using NodeId = int;

using TreeNode = std::map<NodeId, Node*>;
using TreeData = std::tuple<std::string, std::string, std::string>;

class TreeViewModel : public QStandardItemModel
{
    Q_OBJECT

public:
    TreeViewModel(std::vector<NodeData*>&, QObject* parent = nullptr);
    void build_tree_nodes(std::vector<NodeData*>&);
    void insert_node(TreeNode tree_node, Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
    void build_tree(std::vector<Node *> &nodes);

    //Q_INVOKABLE void treeClicked(QModelIndex);
    void treeClicked(QModelIndex);

private:
    std::vector<Node*> nodes;
    QStandardItem* rootItem;
    TreeNode tree;
};

#endif // TREEVIEWMODEL_H
