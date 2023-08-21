#include "readonlydelegate.h"

ReadOnlyDelegate::ReadOnlyDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

bool ReadOnlyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return false;
}

QWidget* ReadOnlyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return Q_NULLPTR;
}
