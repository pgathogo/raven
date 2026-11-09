#pragma once

#include <tuple>
#include <QStandardItemModel>
#include <QTableWidgetItem>

namespace TRAFFIK {
class TraffikNode;
}

using NodeId = int;
using TreeData = std::tuple<std::string, std::string, std::string>;

namespace PIXELPLAN
{
    using SelectedItems = QList<QTableWidgetItem *>;

    struct PPBreak {
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
        using schedule_date = std::string;
        using break_name = std::string;

        void insert(std::string key, int hour, PPBreak value)
        {
           if (m_data.find(key) == m_data.end())
           {
               m_insertion_order.push_back(key);
           }

           m_data[key][hour].push_back(value);
        }

        std::unordered_map<int, std::vector<PPBreak>> get_value(std::string key) {
          return m_data.at(key);
        }

        std::vector<schedule_date> insertion_order(){ return m_insertion_order; }

        int size () { return m_data.size(); }

    private:
        std::vector<schedule_date> m_insertion_order;
        std::unordered_map<std::string, std::unordered_map<int, std::vector<PPBreak>>> m_data;
    };

    using Breaks = OrderedMap;

    class ViewBreaksModel : public QStandardItemModel {
      // Q_OBJECT

    public:
      ViewBreaksModel(QObject *parent = nullptr);
      int read_tree_data(Breaks& items);

      // Prints the model contents to the debug console, indented by depth:
      //   2026-11-01
      //     8
      //       08:00:00 | 08:00:00 | LIVE | 6 | 180 | 2 | 120 | Random
      void print_tree() const;

      TreeData tokenize(std::string line);
      void build_tree(std::vector<TRAFFIK::TraffikNode *> &nodes);

      void make_tree(Breaks&);
      void prepare_tableview();
      void clear_data();

      Q_INVOKABLE void treeClicked(QModelIndex);

    private:
      std::vector<TRAFFIK::TraffikNode *> m_nodes;
      QStandardItem *m_root_item;
    };

}
