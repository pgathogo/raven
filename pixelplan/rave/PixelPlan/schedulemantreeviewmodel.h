#ifndef SCHEDULEMANTREEVIEWMODEL_H
#define SCHEDULEMANTREEVIEWMODEL_H

#include <tuple>

#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace TRAFFIK {
class TraffikNode;
}

using NodeId = int;

using TreeNode = std::map<NodeId, TRAFFIK::TraffikNode *>;
using TreeData = std::tuple<std::string, std::string, std::string>;

using SelectedItems = QList<QTableWidgetItem *>;

struct Break {
  int id{-1};
  std::string schedule_date{};
  std::string comment{};
  int schedule_hour{-1};
  std::string schedule_time{};
  std::string break_mode{};
  std::string break_fill_method{""};
  int max_spots{0};
  double break_duration{0};
  int booked_spots{0};
  double time_left{0};
};

class OrderedMap {
public:
    using hour = int;
    using break_name = std::string;

    void insert(int key, Break value) {
        if (m_data.find(key) == m_data.end()) {

            auto hour_break_name = std::make_tuple(key, value.comment);

            m_insertion_order.push_back(hour_break_name);
        }

        m_data[key].push_back(value);
    }
    std::vector<Break> get_value(int key) {
        return m_data.at(key);
    }

    std::vector<std::tuple<hour, break_name>> insertion_order(){ return m_insertion_order; }

private:
    std::vector<std::tuple<hour, break_name>> m_insertion_order;
    std::unordered_map<int, std::vector<Break>> m_data;
};

//using Breaks = std::map<int, std::vector<Break>>;
using Breaks = OrderedMap;

class ScheduleManTreeViewModel : public QStandardItemModel {
  // Q_OBJECT

public:
  ScheduleManTreeViewModel(Breaks items, QObject *parent = nullptr);
  int read_tree_data(Breaks& items);
  void insert_node(TreeNode tree_node, TRAFFIK::TraffikNode *node);
  void print_tree(TreeNode tree_node, int level = 0);
  TreeData tokenize(std::string line);
  void build_tree(std::vector<TRAFFIK::TraffikNode *> &nodes);

  Q_INVOKABLE void treeClicked(QModelIndex);

private:
  std::vector<TRAFFIK::TraffikNode *> m_nodes;
  QStandardItem *m_root_item;
  TreeNode m_tree;
};

#endif // SCHEDULEMANTREEVIEWMODEL_H
