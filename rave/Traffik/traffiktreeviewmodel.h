#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace TRAFFIK{
    class TraffikNode;
}

using NodeId = int;
using TreeNode = std::map<NodeId, TRAFFIK::TraffikNode*>;
using TreeData = std::tuple<std::string, std::string, std::string>;
using SelectedItems = QList<QTableWidgetItem*>;

namespace TRAFFIK{


    class TraffikTreeViewModel : public QStandardItemModel
    {
       // Q_OBJECT
    public:
        TraffikTreeViewModel(SelectedItems items, QObject* parent = nullptr);
        int read_tree_data(const SelectedItems& items);
        void insert_node(TreeNode tree_node, TraffikNode* node);
        void print_tree(TreeNode tree_node, int level=0);
        TreeData tokenize(std::string line);
        void build_tree(std::vector<TraffikNode *> &nodes);

        //Q_INVOKABLE void treeClicked(QModelIndex);

    private:
        std::vector<TRAFFIK::TraffikNode*> m_nodes;
        QStandardItem* m_root_item;
        TreeNode m_tree;
    };
}

#endif // TREEVIEWMODEL_H
