#include <QTimeEdit>

#include "timeeditdelegate.h"

TimeEditDelegate::TimeEditDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

QWidget* TimeEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTimeEdit* editor = new QTimeEdit(parent);
    editor->setFrame(false);
    editor->setDisplayFormat("HH:mm:ss");

    return editor;
}

void TimeEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QTime value = index.model()->data(index, Qt::EditRole).toTime();
    QTimeEdit* time_edit = static_cast<QTimeEdit*>(editor);
    time_edit->setDisplayFormat("HH:mm:ss");
    time_edit->setTime(value);
}

void TimeEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QTimeEdit* time_edit = static_cast<QTimeEdit*>(editor);
    time_edit->setDisplayFormat("HH:mm:ss");
    time_edit->interpretText();
    QString value = time_edit->time().toString("HH:mm:ss");

    model->setData(index, value, Qt::EditRole);
}

void TimeEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}
