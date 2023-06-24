#pragma once

#include <memory>

#include <QFrame>
#include <QTime>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QString;


namespace OATS{

    class ScheduleItem;

    enum class PanelStatus{WAITING, CUED, PLAYING, PLAYED, STOP};
    enum class ButtonFlashColor{NONE, GREEN, ORANGE, RED};
    enum class ProgressBarBGColor{RED, BLUE, GREEN};

    class OutputPanel : public QFrame
    {
        Q_OBJECT
    public:
        const QString color_red =  "background-color: rgb(210, 4, 45)";

        OutputPanel(QString name);
        ~OutputPanel();

        int id() const;
        void set_id(int);

        QString panel_name();

        QLabel* title();
        QLabel* artist();
        QLabel* time();

        void play();
        void stop();
        void cue_item(ScheduleItem*);

        PanelStatus panel_status();
        void set_panel_status(PanelStatus);
        QString panel_status_text();

        void set_panel_style();

        QTime play_start_time();
        void set_play_start_time(QTime);

        long time_remaining();
        void set_time_remaining(long);
        long long start_tick_stamp();
        void set_start_tick_stamp(long long);
        long long start_trigger_tick_stamp();
        void set_start_trigger_tick_stamp(long long);
        long fade_trigger_tick_stamp();
        void set_fade_trigger_tick_stamp(long long);

        void slow_flash_play_button();
        void fast_flash_play_button(ButtonFlashColor);

        void slow_flash_stop_button();
        void fast_flash_stop_button();

        OATS::ScheduleItem* schedule_item();

        //TimeStamp time_remaining_ts();

        void reset_play_button();
        void reset_stop_button();
        void update_progress_bar(int);
        void set_progress_bar_background(OATS::ProgressBarBGColor);

        void set_cue_time_string(QString);
        QString cue_time_string();

        void delete_cued_item();
    signals:
        void play_item(OutputPanel*);
        void stop_play(OutputPanel*);
    private:
        int m_id;
        QString m_panel_name;
        std::unique_ptr<QPushButton> m_play_button;
        std::unique_ptr<QPushButton> m_stop_button;
        std::unique_ptr<QLabel> m_status_image;
        std::unique_ptr<QLabel> m_title;
        std::unique_ptr<QLabel> m_artist;
        std::unique_ptr<QLabel> m_time;
        std::unique_ptr<QProgressBar> m_progress_bar;

        QHBoxLayout* m_layout_buttons;
        QHBoxLayout* m_layout_title;
        QHBoxLayout* m_layout_artist;
        QHBoxLayout* m_layout_time;

        std::unique_ptr<QVBoxLayout> m_main_panel;

        OATS::ScheduleItem* m_schedule_item;
        PanelStatus m_status;
        QTime m_play_start_time;
        long m_time_remaining{0};
        long long m_start_tick_stamp{0};
        long long m_start_trigger_tick_stamp{0};
        long long m_fade_trigger_tick_stamp{0};

        bool m_slow_flash_bit{false};
        bool m_fast_flash_on{false};

        bool m_stop_slow_flash_bit{false};
        bool m_stop_fast_flash_bit{false};

        QString m_cue_time_string;

        //TimeStamp m_ts;
    };
}
