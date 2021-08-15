#include "tableviewmodel.h"

TableViewModel::TableViewModel(QObject* parent)
    :QStandardItemModel{ parent }
{
    //setHeader();
    //addRow();
}

TableViewModel::~TableViewModel()
{
}

QHash<int, QByteArray> TableViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole] = "one";
    roles[Qt::UserRole+1] = "two";

    return roles;
}

/*
void TableViewModel::setHeader()
{
   QStringList header;
   header << QString("Name") << QString("Artist");
   setHorizontalHeaderLabels(header);
}

void TableViewModel::addRow()
{

    QList<QStandardItem*> cols;

    QStandardItem* track1 = new QStandardItem();
    track1->setData("Dirty Diana", Qt::UserRole);
    track1->setData("Micheal Jackson", Qt::UserRole+1);
    appendRow(track1);

    QStandardItem* track2 = new QStandardItem();
    track2->setData("Sorta Fairytale", Qt::UserRole);
    track2->setData("Tori Amos", Qt::UserRole+1);
    appendRow(track2);

}
*/
