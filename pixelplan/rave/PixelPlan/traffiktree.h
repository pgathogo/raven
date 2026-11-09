#ifndef TREE_H
#define TREE_H

#include <string>
#include <tuple>
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
        int data() const { return m_data; }

        const TreeNode& children() const { return m_children; }
        void add_child(int child_id, TraffikNode* child);

        void add_column(QStandardItem* column);
        QList<QStandardItem*> columns() const;

        // The full model row for this node: the node itself is ALWAYS
        // column 0, followed by any extra columns. This guarantees the
        // TraffikNode is the actual item in the tree, so children can
        // be appended to it later.
        QList<QStandardItem*> row() const;

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
