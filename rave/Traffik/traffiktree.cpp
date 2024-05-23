#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

#include <QDebug>

#include "traffiktree.h"

namespace TRAFFIK{

    TraffikNode::TraffikNode()
        :m_node_name{},
          m_parent_name{},
          m_node_id{-1},
          m_parent_id{-1}
    {
    }

    TraffikNode::TraffikNode(std::string name, std::string parent_name, int n_id, int p_id, int data)
        :m_node_name{name}
         ,m_parent_name{parent_name}
         ,m_node_id{n_id}
         ,m_parent_id{p_id}
         ,m_data{data}
    {
        setText(QString::fromStdString(name));
        setData(data, Qt::UserRole);
    }

    TraffikNode::~TraffikNode(){}

    void TraffikNode::add_column(QStandardItem* column)
    {
        m_columns.push_back(column);
    }

    QList<QStandardItem*> TraffikNode::columns() const
    {
        return m_columns;
    }
}
