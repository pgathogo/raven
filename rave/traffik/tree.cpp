#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

#include <QDebug>

#include "tree.h"

Node::Node()
    :m_node_name{},
      m_parent_name{},
      m_node_id{-1},
      m_parent_id{-1}
{
}

Node::Node(std::string name, std::string parent_name, int n_id, int p_id, int data)
    :m_node_name{name}
     ,m_parent_name{parent_name}
     ,m_node_id{n_id}
     ,m_parent_id{p_id}
     ,m_data{data}
{
    setText(QString::fromStdString(name));
    setData(data, Qt::UserRole);
}

Node::~Node(){}

void Node::add_column(QStandardItem* column)
{
    m_columns.push_back(column);
}

QList<QStandardItem*> Node::columns() const
{
    return m_columns;
}

