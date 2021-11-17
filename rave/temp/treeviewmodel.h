#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>
#include <QTableWidgetItem>

class Node;

using NodeId = int;

using TreeNode = std::map<NodeId, Node*>;
using TreeData = std::tuple<std::string, std::string, std::string>;

using SelectedItems = QList<QTableWidgetItem*>;

class TreeViewModel : public QStandardItemModel
{
   // Q_OBJECT

public:

    TreeViewModel(SelectedItems items, QObject* parent = nullptr);
    int read_tree_data(const SelectedItems& items);
    void insert_node(TreeNode tree_node, Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
    void build_tree(std::vector<Node *> &nodes);

    //Q_INVOKABLE void treeClicked(QModelIndex);

private:
    std::vector<Node*> m_nodes;
    QStandardItem* m_root_item;
    TreeNode m_tree;
};

#endif // TREEVIEWMODEL_H
