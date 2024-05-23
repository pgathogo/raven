#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H


#include <string>
#include <QStandardItemModel>

class Node;
struct NodeData;

using NodeId = int;

using TreeNode = std::map<NodeId, Node*>;
using TreeData = std::tuple<std::string, std::string, std::string>;

class TreeViewModel : public QStandardItemModel
{
    Q_OBJECT

public:
    TreeViewModel(std::vector<NodeData*>, QObject* parent = nullptr);
    ~TreeViewModel();
    void build_tree_nodes(std::vector<NodeData*>&);
    void insert_node(TreeNode tree_node, Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
    void build_tree(std::vector<Node *> &nodes);
    void rebuild_tree();
    //Q_INVOKABLE void treeClicked(QModelIndex);
    void treeClicked(QModelIndex);

    void update_model(NodeData*);
    std::vector<NodeData*> nodes_data();

//    QModelIndex index(int, int, const QModelIndex&) const override;

//    int rowCount(const QModelIndex& parent=QModelIndex()) const;
 //   QVariant data(const QModelIndex&, int) const;

private:
    std::vector<NodeData*> m_node_data;
    std::vector<Node*> nodes;
    QStandardItem* rootItem;
    TreeNode tree;
};

#endif // TREEVIEWMODEL_H
