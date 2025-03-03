#include <iostream>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
//#include <cmath>
#include <algorithm>

#include "audiometer.h"
#include "editor/audioplayer.h"

namespace AUDIO
{
AudioMeter::AudioMeter(std::shared_ptr<AudioPlayer> audio_player, QWidget *parent)
    :m_audio_player(audio_player)
    ,QWidget{parent}
    {
        int width = 280;
        int height = 50;
        m_tick_color = QColor(Qt::black);

        setMinimumSize(width, height);

        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &AudioMeter::update_levels_fft);

        connect(audio_player.get(), &AudioPlayer::sig_start_play, this, &AudioMeter::start_meter);
        connect(audio_player.get(), &AudioPlayer::sig_stop_play, this, &AudioMeter::stop_meter);
        //m_timer->start(50);

        //connect(m_audio_player.get(), &AudioPlayer::current_peak, this, &AudioMeter::update_levels_fft);
    }

    void AudioMeter::start_meter()
    {
        m_timer->start(50);
    }

    void AudioMeter::stop_meter()
    {
        m_timer->stop();
        m_left_level = 0.0;
        m_right_level = 0.0;

        update();
    }

    void AudioMeter::update_levels()
    {
        std::tuple<float, float> levels = m_audio_player->audio_levels();
        auto [m_left_level, m_right_level] = levels;

        if (m_left_level != 0 || m_right_level != 0 ){
            update();
        }
    }

    void AudioMeter::update_levels_fft()
    {

        auto[l_level, r_level] = m_audio_player->audio_levels();

        m_left_level = convert_to_db(l_level);
        m_right_level = convert_to_db(r_level);

        update();
    }

    float AudioMeter::convert_to_db(float level)
    {
        float db = 20 * log10( level / 32768.0f);

        return max(0.0f, min(1.0f, (db + 50) / 50) );

    }

    void AudioMeter::paintEvent(QPaintEvent* event)
    {
        /* Perplexity
         *
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        painter.fillRect(event->rect(), Qt::black);

        int left_bar_height = static_cast<int>(height() * m_left_level);
        painter.fillRect(0, height() - left_bar_height, width()/2, left_bar_height, Qt::green);

        int right_bar_height = static_cast<int>(height() * m_right_level);
        painter.fillRect(width() / 2, height() - right_bar_height, width() / 2, right_bar_height, Qt::green);
        */


        /* Claude  */
        Q_UNUSED(event);
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        int width = this->width() - 10;
        int height = this->height();

        int channel_height = (height - 20) / 2;

        // Draw left channel
        draw_channel(&painter, 0, 0, width, channel_height - 5, m_left_level);

        // Draw right channel
        draw_channel(&painter, 0, channel_height + 15, width, channel_height - 5, m_right_level);

        // Draw labels
        painter.drawText(5, channel_height - 10, "L");
        painter.drawText(5, height - 15, "R");

        draw_tick_marks(&painter, width, channel_height);

        /* Claude */

    }

    void AudioMeter::draw_channel(QPainter* painter, int x, int y, int width, int height, float level)
    {
        int green_width = width * 0.6;   // 60% of width for green
        int yellow_width = width * 0.3;  // 30% of width for yellow
        int red_width = width * 0.1;     // 10% of width for red

        int level_width = width * level;

        int segment_width = 3; // Width of each segment
        int gap_width = 1;     // Width of gap between segments

        // Green section
        draw_segments(painter, x, y, min(level_width, green_width), height, segment_width, gap_width, Qt::green);

        //painter->fillRect(x, y, std::min(level_width, green_width), height, Qt::green);

        // Yellow section
        if (level_width > green_width) {

            draw_segments(painter, x + green_width, y,
                         min(level_width - green_width, yellow_width), height, segment_width, gap_width, Qt::yellow );

            /* smooth bar */
            // painter->fillRect(x + green_width, y,
            //                   std::min(level_width - green_width, yellow_width), height, Qt::yellow);
        }

        if (level_width > green_width + yellow_width) {
            draw_segments(painter, x + green_width + yellow_width, y,
                          min(level_width - green_width - yellow_width, red_width), height, segment_width, gap_width, Qt::red);

            /* smooth bar */
            // painter->fillRect(x + green_width + yellow_width, y,
            //                   std::min(level_width - green_width - yellow_width, red_width), height, Qt::red);
        }

    }

    void AudioMeter::draw_segments(QPainter* painter, int x, int y, int width, int height, int segment_width, int gap_width,
                                       const QColor& color)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);

        int num_segments = width / (segment_width + gap_width);
        for (int i = 0; i < num_segments; ++i) {
            int segmentX = x + i * (segment_width + gap_width);
            painter->drawRect(segmentX, y, segment_width, height);
        }


    }

    void AudioMeter::set_tick_color(const QColor color)
    {
        m_tick_color = color;
    }

    void AudioMeter::draw_tick_marks(QPainter* painter, int width, int channel_height)
    {
        painter->setPen(m_tick_color);
        QFont font = painter->font();
        font.setPointSize(6);
        painter->setFont(font);

        for (int i = 0; i <= 10; i++)
        {
            int x = i * (width / 10 );

            // Top tick mark
            painter->drawLine(x, channel_height, x, channel_height + 5 );

            // Bottom tick mark
            painter->drawLine(x, channel_height * 2 + 15, x, height() - 5 );

            // Draw values
            int db = -50 + i * 5;   // -50 to 0 in steps 0f 5

            QString value = QString::number(db);

            QFontMetrics fm(font);
            int text_width = fm.horizontalAdvance(value);  // + " dB");

            painter->drawText(x - text_width/2, height(), value);  // + " dB");
            //painter->drawText(x - 15, height(), value);

        }

    }


}
