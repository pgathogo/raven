#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QLabel>

#include "commercialviewer.h"

namespace OATS
{

    CommercialViewer::CommercialViewer(QWidget *parent)
      :QWidget(parent)
      ,m_comm_view{nullptr}
      ,m_comm_model{nullptr}
    {
        m_title = std::make_unique<QLabel>();

        m_comm_view = std::make_unique<QTableView>(this);
        m_comm_model = std::make_unique<QStandardItemModel>(this);
        m_comm_view->setModel(m_comm_model.get());

        m_main_layout = new QVBoxLayout();
        m_main_layout->setSpacing(10);
        m_main_layout->addWidget(m_title.get());
        m_main_layout->addWidget(m_comm_view.get());

        create_view_headers();
        setLayout(m_main_layout);
    }

    CommercialViewer::~CommercialViewer()
    {
    }

    void CommercialViewer::create_view_headers()
    {
        m_comm_model->setHorizontalHeaderItem(0, new QStandardItem("Spot Title"));
        m_comm_model->setHorizontalHeaderItem(1, new QStandardItem("Client Name"));
        m_comm_model->setHorizontalHeaderItem(2, new QStandardItem("Spot Duration(secs)"));

        set_view_column_width();
    }

    void CommercialViewer::clear()
    {
        m_comm_model->clear();
    }

    void CommercialViewer::set_view_column_width()
    {
        enum Column{Title, Client, Duration};

        m_comm_view->setColumnWidth(Column::Title, 300);
        m_comm_view->setColumnWidth(Column::Client, 300);
        m_comm_view->setColumnWidth(Column::Duration, 100);
    }

    QStandardItemModel* CommercialViewer::model()
    {
        return m_comm_model.get();
    }

    void CommercialViewer::set_title(QString title)
    {

        m_title->setText(title);

    }
}
