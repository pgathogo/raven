#pragma once

#include <memory>

#include <QWidget>

class QTableView;
class QStandardItemModel;
class QVBoxLayout;
class QLabel;

namespace DATA
{
    class CommercialViewer : public QWidget
    {
        Q_OBJECT
    public:
        explicit CommercialViewer(QWidget *parent = nullptr);
        ~CommercialViewer();
        void clear();

        void create_view_headers();
        void set_view_column_width();
        QStandardItemModel* model();

        void set_title(QString);
        void set_title_font(QFont&);


    private:
        std::unique_ptr<QTableView> m_comm_view;
        std::unique_ptr<QStandardItemModel> m_comm_model;
        QVBoxLayout* m_main_layout;

        std::unique_ptr<QLabel> m_title;

        void style_comm_view();

    //signals:

    };

}

