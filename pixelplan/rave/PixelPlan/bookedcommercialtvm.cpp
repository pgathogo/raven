#include <QStandardItem>

#include "bookedcommercialtvm.h"
#include "traffiktree.h"
#include "../utils/tools.h"

BookedCommercialTreeViewModel::BookedCommercialTreeViewModel(BookedAdverts adverts, QObject* parent)
    :QStandardItemModel{ parent }
{
    setColumnCount(3);
    QStringList columns {"Spot Name", "Client Name", "Duration"};
    setHorizontalHeaderLabels(columns);

    m_root_item = invisibleRootItem();
    read_tree_data(adverts);
    build_tree(m_nodes);
}

int BookedCommercialTreeViewModel::read_tree_data(const BookedAdverts& items)
{
    int parent_id = 0;
    int node_id = 0;
    for(auto& [break_time, booked_adverts]: items)
    {
        std::string t_str = break_time.toString("hh:mm").toStdString();
        std::string parent_title = "Commercial Break: "+t_str;

        TRAFFIK::TraffikNode* parent_node = new TRAFFIK::TraffikNode(parent_title,
                                                                     t_str,
                                                                     ++parent_id,
                                                                     -1,
                                                                     ++node_id);
        m_nodes.push_back(parent_node);
        int child_id = 0;

        for (auto& booked_advert : booked_adverts)
        {
            TRAFFIK::TraffikNode* child_node = new TRAFFIK::TraffikNode(
                booked_advert.spot_name.toStdString(), "",
                ++child_id, parent_id, booked_advert.spot_id);

            QStandardItem* spot_name = new QStandardItem(booked_advert.spot_name);
            QStandardItem* client_name = new QStandardItem(booked_advert.client_name);

            QString spot_duration;

            if (!booked_advert.spot_name.isEmpty())
               spot_duration = duration_to_time(booked_advert.duration).toString("hh:mm:ss");

            QStandardItem* duration = new QStandardItem(spot_duration);

            child_node->add_column(spot_name);
            child_node->add_column(client_name);
            child_node->add_column(duration);

            m_nodes.push_back(child_node);
        }
    }

    return 1;
}

void BookedCommercialTreeViewModel::build_tree(std::vector<TRAFFIK::TraffikNode *>& nodes)
{
    for(auto node : nodes) {
        if (node->parent_id() == -1) {
            m_root_item->appendRow(node);
            m_tree[node->node_id()] = node;
        } else {
            insert_node(m_tree, node);
        }
    }
}

void BookedCommercialTreeViewModel::insert_node(TreeNode tree_node, TRAFFIK::TraffikNode* node)
{
    bool found = false;
    for(auto& [id, parent]: tree_node) {
        if (found) return;
        if (id == node->parent_id()) {
            parent->children()[node->node_id()] = node;
            parent->appendRow(node->columns());
            found = true;
        } else {
            insert_node(parent->children(), node);
        }
    }
}
