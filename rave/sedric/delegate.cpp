#include "delegate.h"
#include "delegateprivate.h"
#include <QDebug>
#include <QPainter>

Delegate::Delegate(QObject* parent)
    :QStyledItemDelegate(parent)
    ,m_delegate_private{new DelegatePrivate}
{

}

Delegate::~Delegate()
{
    delete m_delegate_private;
}

QSize Delegate::icon_size() const
{
    return m_delegate_private->m_icon_size;
}

void Delegate::set_icon_size(int width, int height)
{
    m_delegate_private->m_icon_size = QSize(width, height);
}

QMargins Delegate::contents_margins() const
{
    return m_delegate_private->m_margins;
}

void Delegate::set_contents_margins(int left, int top, int right, int bottom)
{
    m_delegate_private->m_margins = QMargins(left, top, right, bottom);
}

int Delegate::horizontal_spacing() const
{
    return m_delegate_private->m_space_horizontal;
}

void Delegate::set_horizontal_spacing(int spacing)
{
    m_delegate_private->m_space_horizontal = spacing;
}

int Delegate::vertical_spacing() const
{
    return m_delegate_private->m_space_vertical;
}

void Delegate::set_vertical_spacing(int spacing)
{
    m_delegate_private->m_space_vertical = spacing;
}

void Delegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
           const QModelIndex& index) const
{
    qDebug() << "Column: "<< index.column();


    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette& palette(opt.palette);
    const QRect& rect(opt.rect);
    const QRect& content_rect(rect.adjusted(m_delegate_private->m_margins.left(),
                                            m_delegate_private->m_margins.top(),
                                            -m_delegate_private->m_margins.right(),
                                            -m_delegate_private->m_margins.bottom()));

    const bool last_index = (index.model()->rowCount() - 1) == index.row();
    const bool has_icon = !opt.icon.isNull();
    const int bottom_edge = rect.bottom();
    QFont f(opt.font);

    f.setPointSize(m_delegate_private->timestamp_font_point_size(opt.font));

    painter->save();
    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);

    // Draw background
    painter->fillRect(rect, opt.state & QStyle::State_Selected ?
                          palette.highlight().color() :
                          palette.light().color());


    /*
    // Draw bottom line
    painter->setPen(last_index ? palette.dark().color()
                              :palette.mid().color());

    painter->drawLine(last_index ? rect.left() : m_delegate_private->m_margins.left(),
                      bottom_edge, rect.right(), bottom_edge);


    // Draw message icon
    auto pixmap = opt.icon.pixmap(m_delegate_private->m_icon_size);
    if (has_icon){
        painter->drawPixmap(content_rect.left(), content_rect.top(), pixmap);
    }


    // Draw vertical line

    QPoint p1(content_rect.topLeft());
    QPoint p2(content_rect.bottomLeft());

    QLine line(p1.x()+pixmap.rect().width()+5, p1.y()-10,
               p2.x()+pixmap.rect().width()+5, p2.y()+10);

    painter->drawLine(line);


    // Draw timestamp
    QRect timestamp_rect(m_delegate_private->timestamp_box(opt, index));


    timestamp_rect.moveTo(m_delegate_private->m_margins.left() + m_delegate_private->m_icon_size.width()
                          + m_delegate_private->m_space_horizontal, content_rect.top());


    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(timestamp_rect, Qt::TextSingleLine,
                      index.data(Qt::UserRole).toString());

    // Draw message text
    QRect message_rect(m_delegate_private->title_box(opt, index));

    message_rect.moveTo(timestamp_rect.left(), timestamp_rect.bottom()
                       +m_delegate_private->m_space_vertical);

    painter->setFont(opt.font);
    painter->setPen(palette.windowText().color());
    painter->drawText(message_rect, Qt::TextSingleLine,
                      index.data(Qt::UserRole).toString());

    */

    QRect r = option.fontMetrics.boundingRect(index.data(Qt::UserRole).toString()).adjusted(0, 0, 1, 1);

    painter->setFont(opt.font);
    painter->setPen(palette.windowText().color());
    painter->drawText(r, Qt::TextSingleLine,
                      index.data(Qt::UserRole).toString());
    painter->restore();

}

QSize Delegate::sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    int text_height = m_delegate_private->timestamp_box(opt, index).height()
            + m_delegate_private->m_space_vertical
            + m_delegate_private->title_box(opt, index).height();

    int icon_height = m_delegate_private->m_icon_size.height();

    int h = text_height > icon_height ? text_height : icon_height;

//    return QSize(opt.rect.width(), m_delegate_private->m_margins.top() + h
//                 + m_delegate_private->m_margins.bottom());
    return QSize(opt.rect.width(), 40);
}


DelegatePrivate::DelegatePrivate()
    :m_icon_size{12, 12}
    ,m_margins{0, 0 , 0, 0}
    ,m_space_horizontal{0}
    ,m_space_vertical{0}
{
}

QRect DelegatePrivate::timestamp_box(const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
{
    QFont f(option.font);

    f.setPointSizeF(timestamp_font_point_size(option.font));

    return QFontMetrics(f).boundingRect(index.data(Qt::UserRole).toString())
            .adjusted(0, 0, 1, 1);
}

qreal DelegatePrivate::timestamp_font_point_size(const QFont& f) const
{
    return 0.85*f.pointSize();
}

QRect DelegatePrivate::title_box(const QStyleOptionViewItem& option,
                                 const QModelIndex& index) const
{
    return option.fontMetrics.boundingRect(index.data(Qt::UserRole).toString()).adjusted(0, 0, 1, 1);
}

