#include <QComboBox>

#include "comboboxitemdelegate.h"

ComboBoxItemDelegate::ComboBoxItemDelegate(std::map<QString, QVariant> data, QObject *parent)
    : QStyledItemDelegate{parent}
    ,m_data{data}
{

}

ComboBoxItemDelegate::~ComboBoxItemDelegate()
{

}

QWidget* ComboBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    m_combobox = new QComboBox(parent);

    for(auto&[title, value]: m_data){
        m_combobox->addItem(title, value);
    }

    return m_combobox;
}

QComboBox* ComboBoxItemDelegate::get_combobox()
{
    return m_combobox;
}

void ComboBoxItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox* cb =  qobject_cast<QComboBox*>(editor);
    Q_ASSERT(cb);
    // get index of the text that matches the current value of the item
    const QString current_text = index.data(Qt::EditRole).toString();
    const int cb_index = cb->findText(current_text);
    if (cb_index >= 0)
        cb->setCurrentIndex(cb_index);
}

void ComboBoxItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* cb = qobject_cast<QComboBox*>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);
}
