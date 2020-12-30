#include <QStandardItem>
#include <QDebug>

#include "commlogtreeviewmodel.h"
#include "tree.h"
#include "../utils/tools.h"

CommLogTreeViewModel::CommLogTreeViewModel(CommercialLogs items, QObject *parent)
    :QStandardItemModel{ parent }
{
    setColumnCount(7);
    QStringList col_titles {"Client", "Spot Ref", "Spot Title", "Duration", "Status", "Play Date", "Play Time"};
    setHorizontalHeaderLabels(col_titles);

    m_root_item = invisibleRootItem();
    read_tree_data(items);
    build_tree(m_nodes);
    //print_tree(m_tree, 0);
}

int CommLogTreeViewModel::read_tree_data(const CommercialLogs& items)
{
    int parent_id = 0;
    for (auto& [key, bookings] : items){
        Node* parent_node = new Node(key, key, ++parent_id, -1);
        m_nodes.push_back(parent_node);
        int child_id = 0;
        for(auto& booking : bookings){
            Node* child_node = new Node(booking.client_name, booking.spot_name, ++child_id, parent_id);

            QStandardItem* client_name = new QStandardItem(stoq(booking.client_name));
            QStandardItem* spot_id = new QStandardItem(QString::number(booking.spot_id));
            QStandardItem* spot_name = new QStandardItem(stoq(booking.spot_name));
            QStandardItem* spot_duration = new QStandardItem(QString::number(booking.spot_duration));
            QStandardItem* booking_status = new QStandardItem(stoq(booking.booking_status));
            QStandardItem* play_date = new QStandardItem(stoq(booking.play_date));
            QStandardItem* play_time = new QStandardItem(stoq(booking.play_time));

            child_node->add_column(client_name);
            child_node->add_column(spot_id);
            child_node->add_column(spot_name);
            child_node->add_column(spot_duration);
            child_node->add_column(booking_status);
            child_node->add_column(play_date);
            child_node->add_column(play_time);

            m_nodes.push_back(child_node);
        }
    }

 return 1;

}

void CommLogTreeViewModel::build_tree(std::vector<Node *> &nodes)
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

void CommLogTreeViewModel::treeClicked(QModelIndex mindex)
{
    qDebug() << "C++ " << mindex.data(Qt::UserRole).toInt();

}


void CommLogTreeViewModel::insert_node(TreeNode tree_node, Node* node)
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


void CommLogTreeViewModel::print_tree(TreeNode tree_node, int level)
{
    QString indent{""};
    for(int i=0; i<level; ++i)
        indent +="  ";

    for(auto& [id, node] : tree_node){
        //std::cout << indent << "|-" << id <<".:"<< node->name << "(" << node->children.size() << ")" << '\n';
        //qDebug() << indent << "|-" << id << ".:" << QString::fromStdString(node->node_name());
        //print_tree(node->children(), ++level);
        qDebug() << QString::fromStdString(node->node_name());
    }

}


TreeData CommLogTreeViewModel::tokenize(std::string line)
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



