#include <format>
#include <iostream>
#include <functional>
#include <unordered_map>

#include <QStandardItem>
#include <QDebug>

#include "viewbreaksmodel.h"
#include "traffiktree.h"
#include "../utils/tools.h"

namespace PIXELPLAN
{

    ViewBreaksModel::ViewBreaksModel(QObject *parent)
        :QStandardItemModel{ parent }
    {
        m_root_item = invisibleRootItem();

        prepare_tableview();
    }

    void ViewBreaksModel::prepare_tableview()
    {
        setColumnCount(8);
        QStringList col_titles {"Break Hour", "Break Time", "Break Mode", "Max Spots", "Duration", "Booked Spots", "Time Left", "Fill Method"};
        setHorizontalHeaderLabels(col_titles);
    }

    void ViewBreaksModel::clear_data()
    {
        // Items that were appended to the model are owned (and deleted)
        // by the model itself, so only clear our raw-pointer bookkeeping.
        removeRows(0, rowCount());
        m_nodes.clear();
    }

    void ViewBreaksModel::make_tree(Breaks& items)
    {
        if (items.size() > 0 )
        {
            read_tree_data(items);
            build_tree(m_nodes);
        }
    }

    int ViewBreaksModel::read_tree_data(Breaks& items)
    {
        // IMPORTANT: one global id counter for ALL levels. Ids must be
        // unique across the whole tree, otherwise a parent lookup by id
        // can match a node on the wrong level (date vs hour vs break).
        int next_id = 0;

        std::vector<std::string> schedule_dates = items.insertion_order();

        for (const auto& schedule_date : schedule_dates)
        {
            // ---- date node (top level, parent -1) ----
            auto* date_node = new TRAFFIK::TraffikNode(schedule_date, "", ++next_id, -1, 0);
            m_nodes.push_back(date_node);

            auto break_hours = items.get_value(schedule_date);

            for (const auto& [hour, breaks] : break_hours)
            {
                // ---- hour node (child of the date) ----
                std::string hour_title = std::to_string(hour);

                auto* hour_node = new TRAFFIK::TraffikNode(hour_title, schedule_date, ++next_id,
                                                           date_node->node_id(), hour);
                m_nodes.push_back(hour_node);

                date_node->add_child(hour_node->node_id(), hour_node);

                for (const auto& comm_break : breaks)
                {
                    // ---- break node (child of the hour) ----
                    const std::string break_title =
                        comm_break.comment.empty() ? comm_break.schedule_time : comm_break.comment;

                    auto* child_node = new TRAFFIK::TraffikNode(break_title, hour_title,
                                                                ++next_id, hour_node->node_id(), comm_break.id);

                    // The node itself is column 0 (name); these are
                    // columns 1..7, matching the header labels.
                    child_node->add_column(new QStandardItem(stoq(comm_break.schedule_time)));
                    child_node->add_column(new QStandardItem(stoq(comm_break.break_mode)));
                    child_node->add_column(new QStandardItem(QString::number(comm_break.max_spots)));
                    child_node->add_column(new QStandardItem(QString::number(comm_break.break_duration)));
                    child_node->add_column(new QStandardItem(QString::number(comm_break.booked_spots)));
                    child_node->add_column(new QStandardItem(QString::number(comm_break.time_left)));
                    child_node->add_column(new QStandardItem(stoq(comm_break.break_fill_method)));

                    m_nodes.push_back(child_node);

                    hour_node->add_child(child_node->node_id(), child_node);
                }
            }
        }

        return 1;
    }

    void ViewBreaksModel::build_tree(std::vector<TRAFFIK::TraffikNode *> &nodes)
    {
        // Index every node by its (globally unique) id so the parent
        // lookup is O(1) instead of a recursive search through maps
        // whose keys used to collide across levels.
        std::unordered_map<int, TRAFFIK::TraffikNode*> by_id;
        by_id.reserve(nodes.size());
        for (auto* node : nodes)
            by_id[node->node_id()] = node;

        for (auto* node : nodes)
        {
            if (node->parent_id() == -1)
            {
                m_root_item->appendRow(node->row());
            }
            else
            {
                auto it = by_id.find(node->parent_id());
                if (it != by_id.end())
                    it->second->appendRow(node->row());
                else
                    qDebug() << "orphan node (parent not found):" << node->node_id();
            }
        }

        //print_tree();
    }

    void ViewBreaksModel::print_tree() const
    {
        std::function<void(const QStandardItem*, int)> walk =
            [&](const QStandardItem* parent, int depth)
            {
                for (int r = 0; r < parent->rowCount(); ++r)
                {
                    QStringList cells;
                    for (int c = 0; c < columnCount(); ++c)
                    {
                        const QStandardItem* cell = parent->child(r, c);
                        cells << (cell ? cell->text() : QString());
                    }
                    qDebug().noquote() << (QString("  ").repeated(depth) + cells.join(" | "));

                    walk(parent->child(r, 0), depth + 1);
                }
            };

        walk(m_root_item, 0);
    }

    TreeData ViewBreaksModel::tokenize(std::string line)
    {
         size_t pos = 0;
         std::string token;
         std::string delimeter = "|";

         std::vector<std::string> tokens;

         while((pos = line.find(delimeter)) != std::string::npos){
         token = line.substr(0, pos);
         tokens.push_back(token);
         line.erase(0, pos + delimeter.length());
         }

         TreeData tok_tuple = std::make_tuple(tokens.at(0), tokens.at(1), tokens.at(2));

         return tok_tuple;
    }

}
