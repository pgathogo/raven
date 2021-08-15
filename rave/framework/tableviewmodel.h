#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QStandardItemModel>


class TableViewModel : public QStandardItemModel
{
    Q_OBJECT

public:

    TableViewModel(QObject*);
    ~TableViewModel() override;

    QHash<int, QByteArray> roleNames() const override;

    //void setHeader();
    //void addRow();
};

#endif // TABLEVIEWMODEL_H
