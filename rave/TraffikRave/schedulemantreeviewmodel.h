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

struct Break{
    int id{-1};
    std::string schedule_date{};
    int schedule_hour{-1};
    std::string schedule_time{};
    std::string break_mode{};
    int max_spots{0};
    double break_duration{0};
    int booked_spots{0};
    double time_left{0};
};

using Breaks = std::map<int, std::vector<Break>>;

class ScheduleManTreeViewModel : public QStandardItemModel
{
   // Q_OBJECT

public:

    ScheduleManTreeViewModel(Breaks items, QObject* parent = nullptr);
    int read_tree_data(const Breaks& items);
    void insert_node(TreeNode tree_node, TRAFFIK::Node* node);
    void print_tree(TreeNode tree_node, int level=0);
    TreeData tokenize(std::string line);
    void build_tree(std::vector<TRAFFIK::Node *> &nodes);

    Q_INVOKABLE void treeClicked(QModelIndex);

private:
    std::vector<TRAFFIK::Node*> m_nodes;
    QStandardItem* m_root_item;
    TreeNode m_tree;

};

#endif // TREEVIEWMODEL_H
