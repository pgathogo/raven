#ifndef AUDIOMETERVERT_H
#define AUDIOMETERVERT_H

#include <QWidget>

class QTimer;
class QPainter;

namespace AUDIO
{
    class AudioPlayer;

    class AudioMeterVert : public QWidget
    {
        Q_OBJECT
    public:
        explicit AudioMeterVert(std::shared_ptr<AudioPlayer> audio_player, QWidget *parent = nullptr);
        void start_meter();
        void stop_meter();
        void set_tick_color(const QColor);

    signals:

    private slots:
        void update_levels();
        void update_levels_fft();

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        void draw_tick_marks(QPainter*, int, int);
        void draw_channel(QPainter*, int, int, int, int, float);
        void draw_segments(QPainter*, int, int, int, int, int, int, const QColor&, bool);
        float convert_to_db(float);

        std::shared_ptr<AudioPlayer> m_audio_player;
        QTimer* m_timer;
        float m_left_level = 0.0f;
        float m_right_level = 0.0f;

        QColor m_tick_color;
    };
}

#endif // AUDIOMETER_H

