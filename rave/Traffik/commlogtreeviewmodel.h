#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace TRAFFIK{
    class Node;
}

using NodeId = int;

using TreeNode = std::map<NodeId, TRAFFIK::Node*>;
using TreeData = std::tuple<std::string, std::string, std::string>;

using SelectedItems = QList<QTableWidgetItem*>;

struct CommercialLog{
    int id{-1};
    std::string client_name{};
    int spot_id{-1};
    std::string spot_name{};
    double spot_duration{0};
    std::string booking_status{};
    std::string play_date{};
    std::string play_time{};
    std::string schedule_date{};
    std::string schedule_time{};
};

using CommercialLogs = std::map<std::string, std::vector<CommercialLog>>;

class CommLogTreeViewModel : public QStandardItemModel
{
 //   Q_OBJECT

public:

    CommLogTreeViewModel(CommercialLogs items, QObject* parent = nullptr);
    int read_tree_data(const CommercialLogs& items);
    void insert_node(TreeNode tree_node, TRAFFIK::Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
    void build_tree(std::vector<TRAFFIK::Node *> &nodes);

   // Q_INVOKABLE void treeClicked(QModelIndex);

private:
    std::vector<TRAFFIK::Node*> m_nodes;
    QStandardItem* m_root_item;
    TreeNode m_tree;

};



#endif // TREEVIEWMODEL_H
