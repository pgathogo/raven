#include "schedulelist.h"
#include "delegate.h"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDateTime>
#include <QDebug>


ScheduleList::ScheduleList(QWidget* parent)
    :QTableView(parent)
{
    auto* delegate = new Delegate(this);
    QPalette p(palette());

    p.setBrush(QPalette::WindowText, QColor("#303030"));
    p.setBrush(QPalette::Base, QColor("#F0F1F2"));
    p.setBrush(QPalette::Light, QColor("#FFFFFF"));
    p.setBrush(QPalette::Midlight, QColor("#D3D6D8"));
    p.setBrush(QPalette::Mid, QColor("#C5C9Cb"));
    p.setBrush(QPalette::Dark, QColor("#9AA0A4"));
    p.setBrush(QPalette::Text, QColor("#616b71"));
    p.setBrush(QPalette::Highlight, QColor("#E2E4E5"));

    delegate->set_contents_margins(8, 8, 8, 8);
    delegate->set_icon_size(32, 32);
    delegate->set_horizontal_spacing(8);
    delegate->set_vertical_spacing(4);

    setPalette(p);
    setFont(QFont("MSShellDlg 2", 12));
    auto* sim = new QStandardItemModel(1, 2, this);

    sim->setHorizontalHeaderItem(0, new QStandardItem("Col1"));
    sim->setHorizontalHeaderItem(1, new QStandardItem("Col2"));


    setModel(sim);
    setItemDelegate(delegate);

}

void ScheduleList::add_column(QList<QStandardItem*>& cols)
{
//    auto* item = new QStandardItem(row, col);
//    item->setText(text);
    //static_cast<QStandardItemModel*>(model())->appendRow(cols);
    auto* m = static_cast<QStandardItemModel*>(model());

//    for (int row=0; row<m->rowCount(); ++row){
//        for (int col=0; col<m->columnCount(); ++col){
//            QStandardItem* item = new QStandardItem(QString("row %0, column %1").arg(row).arg(col));
//            m->setItem(row, col, item);
//        }
//    }

    QStandardItem* item1 = new QStandardItem("XXX");
    item1->setData("Column1", Qt::UserRole);
//    static_cast<QStandardItemModel*>(model())->setItem(0, 0, item1);
    m->setItem(0, 0, item1);

    QStandardItem* item2 = new QStandardItem("Col2");
    item2->setData("Column2", Qt::UserRole);
//    static_cast<QStandardItemModel*>(model())->setItem(0, 0, item2);
   m->setItem(0, 1, item2);

    scrollToBottom();
}


void ScheduleList::add_item(const QString& text, const QPixmap& pixmap,
                            const QDateTime& datetime)
{
    auto* item = new QStandardItem(QIcon(pixmap), text);

    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    item->setData(datetime.toString("yyyy-MM-dd"), Qt::UserRole);

    static_cast<QStandardItemModel*>(model())->appendRow(item);
    scrollToBottom();
}

void ScheduleList::clear_all()
{
    static_cast<QStandardItemModel*>(model())->clear();
}
