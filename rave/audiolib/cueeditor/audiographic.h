#ifndef AUDIOGRAPHIC_H
#define AUDIOGRAPHIC_H

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGraphicsLineItem>

class QLabel;

namespace AUDIO {

    class AudioWaveScene : public QGraphicsScene
    {
        public:
            AudioWaveScene(QLabel* pos_counter);

            void draw_indicator_line();
            void move_indicator_line(double new_pos);
            void add_text(QPointF pos);
            void change_indicator_position(QPointF pos);
            void set_sec_per_px(double sec_per_px);
            void update_counter(double time);
            void make_indicator_line(QRectF, QLineF);
            void remove_indicator_line();
            QLineF* indicator_line_pos() const;
        private:
            QRectF* m_scene_bound;
            QLineF* m_indicator_line_position;
            QGraphicsLineItem* m_indicator_line_item;
            double m_seconds_per_pixel;
            QLabel* m_position_counter;
            QPen* m_pen;
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
