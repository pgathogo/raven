#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H

#include <map>
#include <QStyledItemDelegate>

class QComboBox;

class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxItemDelegate(std::map<QString, QVariant> data, QObject *parent = nullptr);
    ~ComboBoxItemDelegate();

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    QComboBox* get_combobox();
private:
    mutable QComboBox* m_combobox;
    std::map<QString, QVariant> m_data;
};

#endif // COMBOBOXITEMDELEGATE_H
