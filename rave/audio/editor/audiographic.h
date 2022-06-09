#ifndef AUDIOGRAPHIC_H
#define AUDIOGRAPHIC_H

#include <memory>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include "../../audio/audiofile.h"
#include "subjectobserver.h"

class QLabel;

namespace AUDIO {

    enum MarkerType{Start, FadeIn, Intro, Extro, FadeOut, End};

    using Message = double;

    class MarkerIndicator;

    class MarkerDisplayUnit;

    using Markers = std::map<MarkerType, MarkerIndicator*>;
    using MarkerEx = std::map<MarkerType, std::tuple<std::unique_ptr<MarkerIndicator>,
                                                            std::unique_ptr<MarkerDisplayUnit>>>;

    class MarkerDisplayUnit :  public Observer{
        public:
            MarkerDisplayUnit(MarkerIndicator* indicator = nullptr );
            ~MarkerDisplayUnit() override;
            void update(Subject* changed_subject, double time) override;
            void set_display_unit(QWidget*);
            std::string format_message(Message time) override;
            std::string name() override;
        private:
            QWidget* m_display_unit;
            MarkerIndicator* m_indicator;
    };


    class MarkerIndicator : public Subject
    {
        public:
             MarkerIndicator(MarkerType marker_type);
             ~MarkerIndicator();
             MarkerType marker_type();
             QPen marker_pen();
             void set_line_item(QGraphicsLineItem* line);
             QGraphicsLineItem* line_item();
             void set_line(QLineF);
             QLineF line() const;
             QColor marker_color(MarkerType marker_type);
             void set_current_position_sec(qreal, qreal);
             void set_current_position_px(QPointF);
             double current_position_sec() const;
             QPointF current_position_px() const;
         private:
             QPen m_pen;
             MarkerType m_marker_type;
             QGraphicsLineItem* m_line_item;
             QLineF m_line;
             double m_current_position_in_seconds;
             double m_current_position_in_msec;
             QPointF m_current_position_pixels;
    };


    class AudioWaveScene : public QGraphicsScene
    {
        public:
            AudioWaveScene(QLabel* pos_counter, double audio_length);
            ~AudioWaveScene();

            void draw_indicator_line();
            void move_indicator_line(double new_pos);
            void add_text(QPointF pos);
            void change_indicator_position(QPointF pos);
            void set_sec_per_px(double sec_per_px);
            void update_counter(double time);
            void make_indicator_line(QRectF, QLineF);
            void remove_indicator_line();
            QLineF* indicator_line() const;
            bool is_marker_found(MarkerType);
            MarkerIndicator* find_marker(MarkerType);
            void update_marker(MarkerIndicator*, QLineF);
            void remove_marker_line(MarkerType);
            void add_marker_line(MarkerIndicator*, QLineF);
            void update_marker_line(MarkerIndicator*, QLineF);
            void update_marker_line_position(MarkerType, QLineF);
            void display_marker_position_sec();
            QPointF marker_position(MarkerType);
            double pixel_to_seconds(double px);
            double pixel_to_milli_seconds(double px);
            double seconds_to_pixel(double secs);
            Markers markers() const;

            int count();

            template<typename T1, typename T2>
            T1* create_marker(QWidget display_unit){
                static_assert(std::is_base_of_v<MarkerIndicator, T1>, "`T1` must be derived from MarkerIndicator");
                static_assert(std::is_base_of_v<MarkerDisplayUnit, T2>, "`T2` must be derived from MarkerDisplayUnit");
                auto marker = std::make_unique<T1>();
                auto ptr = marker.get();

                auto mdu = std::make_unique<T2>(ptr);
                mdu->set_display_unit(display_unit);

                m_marker_ex[marker->type_name] = std::make_tuple(std::move(marker), std::move(mdu));

                return ptr;
            }

        private:
            QRectF* m_scene_bound;
            QLineF* m_indicator_line_position;
            QGraphicsLineItem* m_indicator_line_item;
            double m_seconds_per_pixel;
            QLabel* m_position_counter;
            QPen* m_pen;
            QGraphicsLineItem* m_start_marker;

            Markers m_markers;
            MarkerEx m_marker_ex;

            double m_audio_length;
        protected:
            void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    };

    class IndicatorLine :public QGraphicsLineItem
    {
        public:
            IndicatorLine(QRectF bound_rect, QLineF line);
            QRectF boundingRect() const override;
            void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
            QPainterPath shape() const override;
        protected:
            void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
            void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
            void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
            void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        private:
            QRectF m_bounding_rect;
            QRectF m_scene_bounding_rect;
            QLineF m_line;
            QPen* m_pen;
            bool m_hover;
    };


    class AudioGraphic : public QGraphicsView
    {
    public:
        AudioGraphic();
    };


    class Timeline : public AudioGraphic{
        public:
            Timeline(int time);
            ~Timeline();
            void draw_timeline();
            void draw_time_ticks();
            void draw_tick_label(const QString label, qreal x, qreal y);
            double sec_per_pixel() const;
            QGraphicsScene& scene() const;
        private:
            int m_time;
            double m_sec_per_pixel;
            double m_scene_width;
            double m_time_in_secs;

            int m_mins;
            int m_secs;
            QGraphicsScene* m_scene;
            QRectF m_scene_bounds;

            void compute_mins_secs(int time);
            int get_segments(int, int);
    };

    class AudioWave : public AudioGraphic{
        public:
            AudioWave(AudioWaveScene* scene, const QString audio_wave_file, int time_in_secs );
            ~AudioWave();
            void draw_wave();
            QGraphicsScene& scene() const;
            double sec_per_pixel() const;
            void move_indicator_line(double new_pos);
        private:
            QString m_audio_wave_file;
            AUDIO::Timeline* m_timeline;
            AUDIO::AudioWaveScene* m_scene;
            double m_scene_width;
            double m_seconds_per_px;
            double m_time_in_secs;

            QLineF* m_indicator_line_position;
            QGraphicsLineItem* m_indicator_line_item;
            QPen* m_indicator_pen;
            QRectF* m_scene_bound;
    };

}  // AUDIO

#endif // AUDIOGRAPHIC_H
