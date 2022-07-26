#include <map>

#include <QDebug>
#include <QGraphicsTextItem>
#include <QGraphicsItem>
#include <QLineF>
#include <QLabel>
#include <QGraphicsSceneMouseEvent>
#include <QTime>

#include "audiographic.h"
#include "../../audio/audiotool.h"

namespace AUDIO{

    IndicatorLine::IndicatorLine(QRectF bound_rect, QLineF line)
        :m_scene_bounding_rect{ bound_rect },
         m_line{line},
         m_hover{false}

    {
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges |
                 QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
        setAcceptHoverEvents(true);

        m_pen = new QPen(Qt::red);

        m_bounding_rect = bound_rect;

    }

    QRectF IndicatorLine::boundingRect() const
    {
        //return m_bounding_rect;
        return QRectF(m_line.x1()-5, m_line.y1()-5, m_line.dx()+10, m_line.y2()).normalized();
    }

    void IndicatorLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {

        if (!m_hover){
            m_pen->setColor(QColor(Qt::red));
            m_pen->setWidth(1);
        } else {
            m_pen->setColor(QColor(Qt::green));
            m_pen->setWidth(3);
        }

        QLineF line(m_line);
        painter->setPen(*m_pen);
        painter->drawLine(line);

    }

    QPainterPath IndicatorLine::shape() const
    {
        QPainterPath path;
        path.addRect(m_line.x1()-5, m_line.y1()-5, m_line.x2(), m_line.y2());
        return path;
    }

    void IndicatorLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
    {
        m_hover =  true;
        QGraphicsItem::hoverEnterEvent(event);

    }

    void IndicatorLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
    {
        m_hover = false;
        QGraphicsItem::hoverLeaveEvent(event);
    }

    void IndicatorLine::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {

        if (m_line.x1()+x() > 800){
            setX(x());
            setY(0);
            return;
        }

    //    if (x() < 0){
    //        setX(0);
    //        setY(0);
    //        return;
    //    }

        if (y() != 0){
            setY(0);
            return;
        }

        QGraphicsItem::mouseMoveEvent(event);
    }

    void IndicatorLine::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {

        if (m_line.x1() > 800){
            setX(m_line.x1() - x());
            setY(0);
            return;
        }

        if (x() < 0){
           setX(x());
           setY(0);
            return;
        }

        if (y() != 0){
            setY(0);
            return;
        }

        QGraphicsItem::mouseReleaseEvent(event);
    }

    Timeline::Timeline(int time)
        : m_time{time},
          m_scene{nullptr}
    {
        m_scene = new QGraphicsScene(this);
        this->setScene(m_scene);
        this->setMaximumHeight(50);
        this->setMaximumWidth(799);

        m_sec_per_pixel = (m_time*100)/this->width();

        compute_mins_secs(time);

        //draw_timeline();
        draw_time_ticks();

        this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    }

    Timeline::~Timeline()
    {
        //delete m_scene;
    }

    void Timeline::draw_time_ticks()
    {
        QRectF bounds = m_scene->itemsBoundingRect();
        qreal x1 = bounds.x();
        qreal x2 = bounds.x();
        qreal dx;

        qreal y2 = bounds.bottom();

        qreal y1_small = 5;
        qreal y1_big = 10;
        qreal bottom = bounds.bottom(); // bottomLeft().ry();

        int segment = (m_mins > 0) ? 3 : 5;
        double tick_increment = (m_mins > 0) ? 15 : 0.1;
        int total_ticks = (m_mins > 0) ? m_time/5 : m_time / 0.1;
        int total_segments = (m_mins > 0) ? (m_time/5)/segment : (m_time/0.1)/segment;

        int view_width = 800;
        int scene_width = view_width/total_segments;

        QLineF tick, tick2;
    //    tick = QLineF(500, 0, 500, y1_small);

        QPen* pen = new QPen(Qt::red);

        double label_count = 0;
        int tick_count = 0;

        int tick_width = (m_time*100)/scene_width; //(scene_width / total_ticks);

        dx = 0;

        for(int i=1; i< total_ticks; ++i){

    //      dx = scene_width * i;

            if (tick_count < segment){
                m_scene->addLine(dx, bottom, dx, y1_small, *pen);
            }else{
                m_scene->addLine(dx, bottom, dx, y1_big, *pen);
            }

            if (label_count == 0)
                draw_tick_label(QString::number(label_count), dx-7, bottom+5);

            if (tick_count == segment){
                tick_count = 0;
                draw_tick_label(QString::number(label_count), dx-10, bottom+5);
            }
                label_count += tick_increment;
                ++tick_count;

            m_scene->update();

            dx += tick_width;
        }

    }

    void Timeline::draw_tick_label(const QString label, qreal x, qreal y)
    {
        QFont font;
        font.setBold(true);
        QGraphicsTextItem* ti = new QGraphicsTextItem;
        ti->setFont(font);
        ti->setPos(x, y);
        ti->setPlainText(label);
        m_scene->addItem(ti);
    }

    double Timeline::sec_per_pixel() const
    {
        return m_sec_per_pixel;
    }

    QGraphicsScene &Timeline::scene() const
    {
        return *m_scene;
    }

    void Timeline::compute_mins_secs(int time)
    {
        m_mins = time/60;
        m_secs = time % 60;
    }

    int Timeline::get_segments(int mins, int secs)
    {
        return (mins > 0) ? (mins/5)/3 : (secs/0.1)/5;
    }

    void Timeline::draw_timeline()
    {
        QFont font;
        font.setBold(true);
        QGraphicsTextItem* ti = new QGraphicsTextItem;
        ti->setFont(font);
        ti->setPos(0, 0);
        ti->setPlainText("0");
        m_scene->addItem(ti);
    }

    AudioWave::AudioWave(AudioWaveScene* scene, const QString audio_wave_file, int time_in_secs)
        :m_audio_wave_file{audio_wave_file},
         m_timeline{nullptr},
         m_scene{scene}
    {
    //    m_scene = new AudioWaveScene();
        this->setScene(m_scene);
        this->setMaximumHeight(300);
        this->setMaximumWidth(803);


        m_time_in_secs = time_in_secs;
        draw_wave();
        //m_scene->draw_indicator_line();
        this->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    }


    AudioWave::~AudioWave()
    {
        //delete m_timeline;
    }

    void AudioWave::draw_wave()
    {
    //    this->setScene(m_scene);
        qDebug() << "Before :: Time in Secs: "<< m_time_in_secs;

        if (m_time_in_secs == 0.0)
            return;

        qDebug() << "Time in Secs: "<< m_time_in_secs;

        QPixmap pix(m_audio_wave_file);
        m_scene->addPixmap(pix);
        QRectF m_scene_bounds = m_scene->itemsBoundingRect();

        qreal top = m_scene_bounds.top();
        qreal bottom = m_scene_bounds.bottomLeft().ry();

        m_scene_bounds.setWidth(m_scene_bounds.width()*0.9);
        m_scene_bounds.setHeight(m_scene_bounds.height()*0.9);
        this->fitInView(m_scene_bounds, Qt::KeepAspectRatio);

        this->setSceneRect(m_scene_bounds);

        //m_scene_width = m_scene_bounds.width()+m_scene_bounds.width()*0.11;
        m_scene_width = (m_scene_bounds.width()*2)*0.11;
        this->setMaximumWidth(m_scene_width+20);

        m_seconds_per_px = (m_time_in_secs*100)/m_scene_width;
        m_scene->set_sec_per_px(m_seconds_per_px);

        this->centerOn(0, 0);
        this->show();

        qDebug() << "** DONE draw_wave() **";
    }

    QGraphicsScene& AudioWave::scene() const
    {
        return *m_scene;
    }

    double AudioWave::sec_per_pixel() const
    {
        return m_seconds_per_px;
    }

    void AudioWave::move_indicator_line(double new_pos)
    {
        m_scene->move_indicator_line(new_pos);
    }


    /* -------------------- AudioWaveScene ------------------------------- */

    AudioWaveScene::AudioWaveScene(QLabel* pos_counter, double audio_length)
        :m_position_counter{pos_counter}
        ,m_start_marker{nullptr}
        ,m_audio_length{audio_length}
        ,m_scene_bound{nullptr}
        ,m_indicator_line_position{nullptr}
        ,m_indicator_line_item{nullptr}
        ,m_pen{nullptr}
    {
        m_pen = new QPen(Qt::red);
        m_pen->setWidth(5);
    }

    AudioWaveScene::~AudioWaveScene()
    {
        if (m_scene_bound != nullptr)
            delete m_scene_bound;

        if (m_indicator_line_position != nullptr)
            delete m_indicator_line_position;

        if (m_indicator_line_item != nullptr)
            delete m_indicator_line_item;

        if (m_pen != nullptr)
            delete m_pen;
    }


    void AudioWaveScene::draw_indicator_line()
    {

        m_scene_bound = new QRectF(this->itemsBoundingRect());

        m_indicator_line_position = new QLineF((*m_scene_bound).top(), (*m_scene_bound).top(),
                                      (*m_scene_bound).x(), (*m_scene_bound).bottomLeft().ry());

        make_indicator_line(*m_scene_bound, *m_indicator_line_position);

        update();

    }

    void AudioWaveScene::move_indicator_line(double new_pos)
    {
        qreal topx, topy, bx, by;
        topx = m_indicator_line_position->x1();
        bx = m_indicator_line_position->x2();
        topy = m_indicator_line_position->y1();
        by = m_indicator_line_position->y2();

        double dx = (new_pos * 100)/ m_seconds_per_pixel;

        m_indicator_line_position = new QLineF(dx,topy, dx, by);

        remove_indicator_line();

        make_indicator_line(*m_scene_bound, *m_indicator_line_position);

        //double secs = new_pos * m_seconds_per_pixel/ 100;
        update_counter(pixel_to_milli_seconds(dx));
    }

    void AudioWaveScene::add_text(QPointF pos)
    {
        QGraphicsTextItem* ti = new QGraphicsTextItem;
        ti->setPos(pos);
        ti->setPlainText(QString::number(pos.x()));
        this->addItem(ti);
    }

    void AudioWaveScene::change_indicator_position(QPointF pos)
    {
        qreal x1, y1, x2, y2;

        x1 = m_indicator_line_position->x1();
        x2 = m_indicator_line_position->x2();
        y1 = m_indicator_line_position->y1();
        y2 = m_indicator_line_position->y2();

        m_indicator_line_position = new QLineF(pos.x(), y1, pos.x(), y2);

        remove_indicator_line();
        make_indicator_line(*m_scene_bound, *m_indicator_line_position);

//        double secs = pos.x() * m_seconds_per_pixel/ 100;
        update_counter(pixel_to_seconds(pos.x()));
    }

    void AudioWaveScene::set_sec_per_px(double sec_per_px)
    {
        m_seconds_per_pixel = sec_per_px;
    }


    void AudioWaveScene::update_counter(double time)
    {
        QString time_str;

        if (time < 0){
            time_str = QTime::fromMSecsSinceStartOfDay(time).toString("hh:mm:ss");
            m_position_counter->setText(time_str);
            return;
        }

        time_str = QTime::fromMSecsSinceStartOfDay(time).toString("hh:mm:ss");
        m_position_counter->setText(time_str);

    }

    void AudioWaveScene::make_indicator_line(QRectF scene_bound, QLineF line_pos)
    {
        m_indicator_line_item = new IndicatorLine(scene_bound, line_pos);
        addItem(m_indicator_line_item);
    }

    int AudioWaveScene::count()
    {
        return items().size();
    }

    void AudioWaveScene::remove_indicator_line()
    {
        removeItem(m_indicator_line_item);
        delete m_indicator_line_item;
    }

    QLineF *AudioWaveScene::indicator_line() const
    {
        return m_indicator_line_position;
    }

    bool AudioWaveScene::is_marker_found(MarkerType marker_type)
    {
        std::map<MarkerType, MarkerIndicator*>::iterator it;
        it = m_markers.find(marker_type);
        return (it != m_markers.end()) ? true : false;
    }

    MarkerIndicator* AudioWaveScene::find_marker(MarkerType marker_type)
    {
        std::map<MarkerType, MarkerIndicator*>::iterator it;
        it = m_markers.find(marker_type);
        return (it != m_markers.end()) ? (*it).second : nullptr;
    }


    void AudioWaveScene::update_marker(MarkerIndicator* marker, QLineF line)
    {
        remove_marker_line(marker->marker_type());
        update_marker_line(marker, line);
    }

    void AudioWaveScene::remove_marker_line(MarkerType marker_type)
    {
        removeItem(m_markers[marker_type]->line_item());
    }

    void AudioWaveScene::add_marker_line(MarkerIndicator* marker, QLineF line)
    {
        auto item = addLine(line, marker->marker_pen());
        marker->set_line_item(item);
        marker->set_line(line);
        m_markers[marker->marker_type()] = marker;
        update_marker_line_position(marker->marker_type(), line);
    }

    void AudioWaveScene::update_marker_line(MarkerIndicator* marker, QLineF line)
    {
        auto item = addLine(line, marker->marker_pen());
        m_markers[marker->marker_type()]->set_line_item(item);
        m_markers[marker->marker_type()]->set_line(line);
        update_marker_line_position(marker->marker_type(), line);
    }

    void AudioWaveScene::update_marker_line_position(MarkerType marker_type, QLineF line)
    {
        m_markers[marker_type]->set_current_position_px(line.p1());
        m_markers[marker_type]->set_current_position_sec(line.x1(), m_seconds_per_pixel);
    }

    void AudioWaveScene::display_marker_position_sec()
    {
        for (auto& [marker_type, marker] : m_markers){
            qDebug() << marker_type << marker->current_position_sec();
        }
    }

    QPointF AudioWaveScene::marker_position(MarkerType marker_type)
    {
        auto marker = find_marker(marker_type);
        if (marker != nullptr){
            return m_markers[marker_type]->current_position_px();
        }else{
            return QPointF();
        }
    }

    double AudioWaveScene::pixel_to_seconds(double px)
    {
        return px * m_seconds_per_pixel/ 100;
    }

    double AudioWaveScene::pixel_to_milli_seconds(double px)
    {
        return (px * m_seconds_per_pixel/ 100)*1000;
    }

    double AudioWaveScene::seconds_to_pixel(double secs)
    {
        if (secs == 0)
            return 0;

        return ((secs * 100) / ((m_audio_length * 100)/800));
    }

    Markers AudioWaveScene::markers() const
    {
        return m_markers;
    }

    void AudioWaveScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        change_indicator_position(event->scenePos());
        QGraphicsScene::mousePressEvent(event);
        update();
    }

    AudioGraphic::AudioGraphic()
    {

    }

   MarkerIndicator::MarkerIndicator(MarkerType marker_type)
          :m_marker_type{marker_type},
          m_line_item{nullptr}
    {
        m_pen = marker_pen();
        m_pen.setWidth(5);
    }

    MarkerIndicator::~MarkerIndicator()
    {
    }

    MarkerType MarkerIndicator::marker_type()
    {
        return m_marker_type;
    }

    QPen MarkerIndicator::marker_pen()
    {
        return QPen(marker_color(m_marker_type));
    }

    void MarkerIndicator::set_line_item(QGraphicsLineItem* line)
    {
        m_line_item = line;
    }

    QGraphicsLineItem* MarkerIndicator::line_item()
    {
        return m_line_item;
    }

    void MarkerIndicator::set_line(QLineF line)
    {
        m_line = line;
    }

    QLineF MarkerIndicator::line() const
    {
        return m_line;
    }

    QColor MarkerIndicator::marker_color(MarkerType marker_type)
    {
        switch (marker_type)
        {
          case MarkerType::Start:
            return QColor(0, 255, 0);
          case MarkerType::FadeIn:
            return QColor(213, 213, 0);
          case MarkerType::Intro:
            return QColor(55, 55, 166);
          case MarkerType::FadeOut:
            return QColor(127, 127, 0);
          case MarkerType::Extro:
            return QColor(88, 176, 130);
          case MarkerType::End:
            return QColor(212, 0, 0);
          default:
            return QColor(Qt::green);
        }
    }

    void MarkerIndicator::set_current_position_sec(qreal x_pos, qreal secs_per_px)
    {
        m_current_position_in_seconds =  x_pos * secs_per_px / 100;
        m_current_position_in_msec = m_current_position_in_seconds * 1000;
        this->notify(m_current_position_in_msec);
    }

    void MarkerIndicator::set_current_position_px(QPointF pos)
    {
        m_current_position_pixels = pos;
    }

    double MarkerIndicator::current_position_sec() const
    {
        return m_current_position_in_seconds;
    }

    QPointF MarkerIndicator::current_position_px() const
    {
        return m_current_position_pixels;
    }


    MarkerDisplayUnit::MarkerDisplayUnit(MarkerIndicator* indicator)
        : m_display_unit{nullptr},
          m_indicator{indicator}
    {
        m_indicator->attach(this);
    }

    MarkerDisplayUnit::~MarkerDisplayUnit()
    {
    }

    void MarkerDisplayUnit::update(Subject *changed_subject, double time)
    {
        if (m_display_unit != nullptr){
            AUDIO::AudioTool at;
            auto du = dynamic_cast<QLabel*>(m_display_unit);
            du->setText(at.format_time(time));
        }
    }

    void MarkerDisplayUnit::set_display_unit(QWidget* display_unit)
    {
        m_display_unit = display_unit;
    }

    std::string MarkerDisplayUnit::format_message(Message time)
    {
        const int ms_per_second =  1000;
        QString mill_str;
        int mill_count=0;

        QString time_format="%1:%2:%3:%4";
        QString time_str;

        int r_count=0;

        int msecs=0;
        if (time < 0){
            msecs = time * ms_per_second;
            msecs = msecs% 1000;
            time_str = time_format.arg(0)
                    .arg(0, 2, 10, QChar('0'))
                    .arg(0, 2, 10, QChar('0'))
                    .arg(msecs, 3, 10, QChar('0'));

           return time_str.toStdString();
        }

        int msec_d = time * 1000;
        msecs = msec_d % 1000;
        int mins = time / 60;
        int secs = (int)time % 60;

        time_str = time_format.arg(0)
                .arg(mins, 2, 10, QChar('0'))
                .arg(secs, 2, 10, QChar('0'))
                .arg(msecs, 3, 10, QChar('0'));

        return time_str.toStdString();

    }

    std::string MarkerDisplayUnit::name()
    {
        return "Nemo";
    }


} // AUDIO
