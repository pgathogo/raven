#pragma once

#include <QStyledItemDelegate>

class DelegatePrivate;

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    Delegate(QObject* parent=nullptr);
    ~Delegate();

    QSize icon_size() const;
    void set_icon_size(int, int);

    QMargins contents_margins() const;
    void set_contents_margins(int, int, int, int);

    int horizontal_spacing() const;
    void set_horizontal_spacing(int);

    int vertical_spacing() const;
    void set_vertical_spacing(int);

    void paint(QPainter* painter, const QStyleOptionViewItem& option,
              const QModelIndex& index) const override;

    QSize sizeHint(const QStyleOptionViewItem& option,
                  const QModelIndex& index) const override;

private:
    DelegatePrivate* m_delegate_private;

};

