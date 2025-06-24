#include <iostream>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
//#include <cmath>
#include <algorithm>

#include "audiometervert.h"
#include "editor/audioplayer.h"

namespace AUDIO
{
AudioMeterVert::AudioMeterVert(std::shared_ptr<AudioPlayer> audio_player, QWidget *parent)
    :m_audio_player(audio_player)
    ,QWidget{parent}
    {
        int width = 50; // Changed to 50 for vertical
        int height = 280; // Changed to 280 for vertical
        m_tick_color = QColor(Qt::black);

        setMinimumSize(width, height);

        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &AudioMeterVert::update_levels_fft);

        connect(audio_player.get(), &AudioPlayer::sig_start_play, this, &AudioMeterVert::start_meter);
        connect(audio_player.get(), &AudioPlayer::sig_stop_play, this, &AudioMeterVert::stop_meter);
    }

    void AudioMeterVert::start_meter()
    {
        m_timer->start(50);
    }

    void AudioMeterVert::stop_meter()
    {
        m_timer->stop();
        m_left_level = 0.0;
        m_right_level = 0.0;

        update();
    }

    void AudioMeterVert::update_levels()
    {
        std::tuple<float, float> levels = m_audio_player->audio_levels();
        auto [m_left_level, m_right_level] = levels;

        if (m_left_level != 0 || m_right_level != 0 ){
            update();
        }
    }

    void AudioMeterVert::update_levels_fft()
    {

        auto[l_level, r_level] = m_audio_player->audio_levels();

        m_left_level = convert_to_db(l_level);
        m_right_level = convert_to_db(r_level);

        update();
    }

    float AudioMeterVert::convert_to_db(float level)
    {
        float db = 20 * log10( level / 32768.0f);

        return max(0.0f, min(1.0f, (db + 50) / 50) );

    }

    void AudioMeterVert::paintEvent(QPaintEvent* event)
    {

        Q_UNUSED(event);
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);

        int width = this->width() ;
        int height = this->height() - 10; // Adjusted height

        int channel_width = (width - 20) / 2; // Adjusted for vertical layout

        // Draw left channel
        draw_channel(&painter, 0, height/2 + 10, channel_width -5 ,height/2, m_left_level); //adjusted positions and size

        // Draw right channel
        draw_channel(&painter, channel_width + 15, height/2 +10, channel_width-5 , height/2, m_right_level); //adjusted positions and size

        // Draw labels
        painter.drawText(5, height/2, "L"); //adjusted positions
        painter.drawText(channel_width +20, height/2, "R");//adjusted positions

        draw_tick_marks(&painter, width, height);

    }

    void AudioMeterVert::draw_channel(QPainter* painter, int x, int y, int width, int height, float level)
    {
        int green_height = height * 0.6;   // 60% of height for green
        int yellow_height = height * 0.3;  // 30% of height for yellow
        int red_height = height * 0.1;     // 10% of height for red

        int level_height = height * level;

        int segment_height = 3; // height of each segment
        int gap_height = 1;     // height of gap between segments
        int current_y = y + height;


        // Green section
         draw_segments(painter, x, current_y - min(level_height, green_height) , width, min(level_height, green_height), segment_height, gap_height, Qt::green, true);

        // Yellow section
        if (level_height > green_height) {
             draw_segments(painter, x, current_y - green_height - min(level_height - green_height, yellow_height), width, min(level_height - green_height, yellow_height), segment_height, gap_height, Qt::yellow, true);
        }

        if (level_height > green_height + yellow_height) {
             draw_segments(painter, x, current_y - green_height - yellow_height - min(level_height - green_height - yellow_height, red_height), width, min(level_height - green_height - yellow_height, red_height), segment_height, gap_height, Qt::red, true);
        }
    }

     void AudioMeterVert::draw_segments(QPainter* painter, int x, int y, int width, int height, int segment_height, int gap_height,
                                        const QColor& color, bool vertical)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);

         if(vertical){
            int num_segments = height / (segment_height + gap_height);

            for (int i = 0; i < num_segments; ++i) {
                int segmentY = y + i * (segment_height + gap_height);
                painter->drawRect(x, segmentY, width, segment_height);
            }
        }else {
            int num_segments = width / (segment_height + gap_height);
            for (int i = 0; i < num_segments; ++i) {
                int segmentX = x + i * (segment_height + gap_height);
                painter->drawRect(segmentX, y, segment_height, height);
            }
         }
    }

    void AudioMeterVert::set_tick_color(const QColor color)
    {
        m_tick_color = color;
    }

    void AudioMeterVert::draw_tick_marks(QPainter* painter, int width, int height)
    {
        painter->setPen(m_tick_color);
        QFont font = painter->font();
        font.setPointSize(6);
        painter->setFont(font);
        int channel_width = (width - 20) / 2;
        int channel_height = height/2;

        for (int i = 0; i <= 10; i++)
        {

            int y = (height) - i * (height / 10 );
            //int y = i * (height / 10 );


            // Top tick mark
            painter->drawLine(channel_width, y , channel_width + 5, y );

            // Bottom tick mark
            painter->drawLine( channel_width * 2 +15 , y,  channel_width * 2 + 10, y );

            // Draw values
            int db = -50 + i * 5;   // -50 to 0 in steps 0f 5

            QString value = QString::number(db);

            QFontMetrics fm(font);
            int text_width = fm.horizontalAdvance(value);  // + " dB");

            painter->drawText( width/2 - text_width/2, y + fm.height()/2 , value );  // + " dB");
        }

    }


}

