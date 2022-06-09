#ifndef MARKINDICATOR_H
#define MARKINDICATOR_H

#include<QPen>
#include<QGraphicsLineItem>

#include "subjectobserver.h"

template<typename T1>
class MarkerIndicatorTmpl : public Subject
{
    public:
         MarkerIndicatorTmpl();
         virtual ~MarkerIndicatorTmpl();
         virtual QPen marker_pen() = 0;
         void set_line_item(QGraphicsLineItem* line);
         QGraphicsLineItem* line_item();
         void set_line(QLineF);
         QLineF line() const;
         virtual QColor marker_color() = 0;
         void set_current_position_sec(qreal, qreal);
         void set_current_position_px(QPointF);
         double current_position_sec() const;
         QPointF current_position_px() const;
     private:
         QPen m_pen;
         QGraphicsLineItem* m_line_item;
         QLineF m_line;
         double m_current_position_in_seconds;
         QPointF m_current_position_pixels;
};



template<typename T>
void MarkerIndicatorTmpl<T>::set_line_item(QGraphicsLineItem* line)
{
    m_line_item = line;
}

template<typename T>
MarkerIndicatorTmpl<T>::MarkerIndicatorTmpl()
{
}

template<typename T>
QGraphicsLineItem* MarkerIndicatorTmpl<T>::line_item()
{
    return m_line_item;
}

template<typename T>
void MarkerIndicatorTmpl<T>::set_line(QLineF line)
{
    m_line = line;
}

template<typename T>
QLineF MarkerIndicatorTmpl<T>::line() const
{
    return m_line;
}

template<typename T>
QColor MarkerIndicatorTmpl<T>::marker_color()
{
    return Qt::green;
}

template<typename T>
void MarkerIndicatorTmpl<T>::set_current_position_sec(qreal x_pos, qreal secs_per_px)
{
    m_current_position_in_seconds =  x_pos * secs_per_px / 100;
    this->notify(m_current_position_in_seconds);
}

template<typename T>
void MarkerIndicatorTmpl<T>::set_current_position_px(QPointF pos)
{
    m_current_position_pixels = pos;
}

template<typename T>
double MarkerIndicatorTmpl<T>::current_position_sec() const
{
    return m_current_position_in_seconds;
}

template<typename T>
QPointF MarkerIndicatorTmpl<T>::current_position_px() const
{
    return m_current_position_pixels;
}

template<typename T>
class StartMarkerIndicator : public MarkerIndicatorTmpl<T>
{
    public:
        StartMarkerIndicator();
        ~StartMarkerIndicator();
        QPen marker_pen() override;
        QColor marker_color() override;
};

template<typename T>
StartMarkerIndicator<T>::StartMarkerIndicator()
{
}

template<typename T>
StartMarkerIndicator<T>::~StartMarkerIndicator()
{
}

template<typename T>
QPen StartMarkerIndicator<T>::marker_pen()
{
    return QPen(marker_color());
}

template<typename T>
QColor StartMarkerIndicator<T>::marker_color()
{
    return QColor(Qt::green);
}


#endif // MARKINDICATOR_H
