#pragma once

#include <qglobal.h>

class DelegatePrivate
{
    DelegatePrivate();

    inline QRect timestamp_box(const QStyleOptionViewItem& option,
                               const QModelIndex& index) const;
    inline qreal timestamp_font_point_size(const QFont& f)const;
    inline QRect title_box(const QStyleOptionViewItem& option,
                           const QModelIndex& index) const;
    inline QRect artist_box(const  QStyleOptionViewItem& option,
                            const QModelIndex& index) const;

    QSize m_icon_size;
    QMargins m_margins;
    int m_space_horizontal;
    int m_space_vertical;

    friend class Delegate;
};
