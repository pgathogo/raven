#pragma once

#include <QTableView>

class QStandardItem;

class ScheduleList : public QTableView
{
    Q_OBJECT
public:
    explicit ScheduleList(QWidget* parent=nullptr);

public slots:
    void add_item(const QString& text, const QPixmap& pixmap,
                  const QDateTime& datetime);
    void add_column(QList<QStandardItem*>&);
    void clear_all();

};

