#include <QStandardItem>
#include <QDebug>

#include "schedulemantreeviewmodel.h"
#include "traffiktree.h"
#include "../utils/tools.h"

ScheduleManTreeViewModel::ScheduleManTreeViewModel(Breaks items, QObject *parent)
    :QStandardItemModel{ parent }
{
    setColumnCount(7);
    QStringList col_titles {"Schedule Time", "Break Mode", "Max Spots", "Duration", "Booked Spots", "Time Left", "Fill Method"};
    setHorizontalHeaderLabels(col_titles);

    m_root_item = invisibleRootItem();
    read_tree_data(items);
    build_tree(m_nodes);
}

int ScheduleManTreeViewModel::read_tree_data(const Breaks& items)
{
    int parent_id = 0;
    for (auto& [hour, breaks] : items){
        TRAFFIK::TraffikNode* parent_node = new TRAFFIK::TraffikNode("Hour: "+std::to_string(hour),
                                     std::to_string(hour), ++parent_id, -1, hour);
        m_nodes.push_back(parent_node);
        int child_id = 0;
        for(auto& comm_break : breaks){
            TRAFFIK::TraffikNode* child_node = new TRAFFIK::TraffikNode(comm_break.schedule_time, "",
                                        ++child_id, parent_id, comm_break.id);

            QStandardItem* schedule_time = new QStandardItem(stoq(comm_break.schedule_time));
            QStandardItem* break_mode = new QStandardItem(stoq(comm_break.break_mode));
            QStandardItem* max_spots = new QStandardItem(QString::number(comm_break.max_spots));
            QStandardItem* duration = new QStandardItem(QString::number(comm_break.break_duration));
            QStandardItem* booked_spots = new QStandardItem(QString::number(comm_break.booked_spots));
            QStandardItem* time_left = new QStandardItem(QString::number(comm_break.time_left));
            QStandardItem* fill_method = new QStandardItem(stoq(comm_break.break_fill_method));

            child_node->add_column(schedule_time);
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
    for (auto node : nodes){
        if (node->parent_id() == -1){
            m_root_item->appendRow(node);
            m_tree[node->node_id()] = node;
        }else {
            insert_node(m_tree, node);
        }
    }

}

//void ScheduleManTreeViewModel::treeClicked(QModelIndex mindex)
//{
//    qDebug() << "C++ " << mindex.data(Qt::UserRole).toInt();
//}


void ScheduleManTreeViewModel::insert_node(TreeNode tree_node, TRAFFIK::TraffikNode* node)
{
    bool found=false;
    for(auto& [id, parent] : tree_node){
        if (found) return;
        if (id == node->parent_id()){
            parent->children()[node->node_id()] = node;
            parent->appendRow(node->columns());
            found = true;
        }else{
            insert_node(parent->children(), node);
        }
    }
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

