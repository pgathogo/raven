#ifndef TREE_H
#define TREE_H

#include <string>
#include <map>
#include <QStandardItem>

namespace TRAFFIK{

    class TraffikNode;

    using NodeID = int;
    using TreeNode = std::map<NodeID, TRAFFIK::TraffikNode*>;
    using TreeData = std::tuple<std::string, std::string, std::string>;

    class TraffikNode : public QStandardItem{
    public:
        TraffikNode();
        TraffikNode(std::string name, std::string parent_name, int n_id, int p_id, int data);
        ~TraffikNode();

        std::string node_name() const { return m_node_name; }
        std::string parent_name() const { return m_parent_name; }
        int node_id() const { return m_node_id; }
        int parent_id() const { return m_parent_id; }
        TreeNode children(){ return m_children; }

        void add_column(QStandardItem* column);
        QList<QStandardItem*> columns() const;

    private:
        std::string m_node_name;
        std::string m_parent_name;
        int m_node_id;
        int m_parent_id;
        int m_data;
        TreeNode m_children;
        QList<QStandardItem*> m_columns;

    };
}

#endif // TREE_H
