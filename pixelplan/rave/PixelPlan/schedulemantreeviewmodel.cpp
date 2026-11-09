#include <format>

#include <QStandardItem>
#include <QDebug>

#include "schedulemantreeviewmodel.h"
#include "traffiktree.h"
#include "../utils/tools.h"

ScheduleManTreeViewModel::ScheduleManTreeViewModel(QObject *parent)
    :QStandardItemModel{ parent }
{
    m_root_item = invisibleRootItem();

    prepare_tableview();
}

void ScheduleManTreeViewModel::prepare_tableview()
{
    setColumnCount(8);
    QStringList col_titles {"Break Hour", "Break Time", "Break Mode", "Max Spots", "Duration", "Booked Spots", "Time Left", "Fill Method"};
    setHorizontalHeaderLabels(col_titles);
}
void ScheduleManTreeViewModel::clear_data()
{
    removeRows(0, rowCount());
    m_nodes.clear();
}

void ScheduleManTreeViewModel::make_tree(Breaks& items)
{
    if (items.size() > 0 ) {
        read_tree_data(items);
        build_tree(m_nodes);
    }
}

int ScheduleManTreeViewModel::read_tree_data(Breaks& items)
{
    int parent_id = 0;
    std::vector<std::tuple<int, std::string>> hours = items.insertion_order();

    for ( const auto& hour_break_name : hours) {

        auto [hour, break_name] = hour_break_name;

        std::string title = std::format("{}", std::to_string(hour));

        TRAFFIK::TraffikNode* parent_node = new TRAFFIK::TraffikNode(title,
                                     std::to_string(hour), ++parent_id, -1, hour);

        m_nodes.push_back(parent_node);
        int child_id = 0;

        auto breaks = items.get_value(hour);

        for(const auto& comm_break : breaks)
        {

            TRAFFIK::TraffikNode* child_node = new TRAFFIK::TraffikNode(comm_break.schedule_time, "",
                                        ++child_id, parent_id, comm_break.id);

            // std::string str_time = std::format("{}: {}", comm_break.comment, comm_break.schedule_time);
            //QStandardItem* schedule_time = new QStandardItem(stoq(str_time));

            QStandardItem* break_name_item = new QStandardItem(stoq(comm_break.comment));
            QStandardItem* break_time = new QStandardItem(stoq(comm_break.schedule_time));

            QStandardItem* break_mode = new QStandardItem(stoq(comm_break.break_mode));
            QStandardItem* max_spots = new QStandardItem(QString::number(comm_break.max_spots));
            QStandardItem* duration = new QStandardItem(QString::number(comm_break.break_duration));
            QStandardItem* booked_spots = new QStandardItem(QString::number(comm_break.booked_spots));
            QStandardItem* time_left = new QStandardItem(QString::number(comm_break.time_left));
            QStandardItem* fill_method = new QStandardItem(stoq(comm_break.break_fill_method));

            //child_node->add_column(schedule_time);
            child_node->add_column(break_name_item);
            child_node->add_column(break_time);
            child_node->add_column(break_mode);
            child_node->add_column(max_spots);
            child_node->add_column(duration);
            child_node->add_column(booked_spots);
            child_node->add_column(time_left);
            child_node->add_column(fill_method);

            m_nodes.push_back(child_node);
        }
    }

 return 1;

}

void ScheduleManTreeViewModel::build_tree(std::vector<TRAFFIK::TraffikNode *> &nodes)
{
    /*
    for (auto node : nodes){
        if (node->parent_id() == -1){
            m_root_item->appendRow(node);
            m_tree[node->node_id()] = node;
        }else {
            insert_node(m_tree, node);
        }
    }
    */

        for (auto* node : nodes)
        {
            if (node->parent_id() == -1)
            {
                m_root_item->appendRow(node->row());
                m_tree[node->node_id()] = node;
            }
            else
            {
                insert_node(m_tree, node);
            }
        }

        //print_tree();
}


bool ScheduleManTreeViewModel::insert_node(const TreeNode& tree, TRAFFIK::TraffikNode* node)
{
    /*
    bool found=false;

    for(auto& [id, parent] : tree_node)
    {
        if (found) return;

        if (id == node->parent_id()){
            parent->children()[node->node_id()] = node;
            parent->appendRow(node->columns());
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
    */
        // Fast path: is the parent in this (sub)tree level?
        auto it = tree.find(node->parent_id());
        if (it != tree.end())
        {
            // node->row() has the TraffikNode itself as column 0, so the
            // node becomes a real item in the view and can hold children.
            it->second->appendRow(node->row());
            return true;
        }

        // Otherwise descend into each child's children (by reference,
        // no map copies).
        for (const auto& [id, parent] : tree)
        {
            if (insert_node(parent->children(), node))
                return true;
        }

        return false;
}


void ScheduleManTreeViewModel::print_tree(TreeNode tree_node, int level)
{
    QString indent{""};
    for(int i=0; i<level; ++i)
        indent +="  ";

    for(auto& [id, node] : tree_node){
        qDebug() << QString::fromStdString(node->node_name());
    }

}


TreeData ScheduleManTreeViewModel::tokenize(std::string line)
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

