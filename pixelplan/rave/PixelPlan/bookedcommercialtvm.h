#ifndef BOOKEDCOMMERCIALTREEVIEWMODEL_H
#define BOOKEDCOMMERCIALTREEVIEWMODEL_H

#include <QStandardItemModel>
#include <QDate>

namespace TRAFFIK {
class TraffikNode;
}

using NodeId = int;

using TreeNode = std::map<NodeId, TRAFFIK::TraffikNode*>;
using TreeData = std::tuple<std::string, std::string, std::string>;


struct BookedAdvert {
    QDate booked_date;
    QTime booked_time;
    QString break_title;
    QString spot_name{};
    QString client_name{};
    int duration=0;
    int spot_id=-1;
    QString filename{};
    QString media_path{};
    QString file_extension{};
    QString filepath{};
};

using BreakTitle = std::map<QTime, QString>;


using BookedAdverts = std::map<QTime, std::vector<BookedAdvert>>;

class BookedCommercialTreeViewModel: public QStandardItemModel {

public:
    BookedCommercialTreeViewModel(BookedAdverts, BreakTitle, QObject* parent=nullptr);
    int read_tree_data(const BookedAdverts&, BreakTitle&);
    void insert_node(TreeNode, TRAFFIK::TraffikNode*);
    void print_tree(TreeNode, int level=0);
    TreeData tokenize(std::string);
    void build_tree(std::vector<TRAFFIK::TraffikNode*>&);

    Q_INVOKABLE void treeClicked(QModelIndex);
private:
    std::vector<TRAFFIK::TraffikNode*> m_nodes;
    QStandardItem* m_root_item;
    TreeNode m_tree;
};


#endif  // BOOKEDCOMMERCIALTREEVIEWMODEL_H
