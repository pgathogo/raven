#ifndef TIMEEDITDELEGATE_H
#define TIMEEDITDELEGATE_H

#include <QStyledItemDelegate>

class TimeEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TimeEditDelegate(QObject *parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

#endif // TIMEEDITDELEGATE_H
