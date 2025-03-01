#pragma once

#include <memory>

#include <QWidget>
#include <QHBoxLayout>

class QLabel;
class QPushButton;

namespace OATS {

    enum class PlayMode {Stop, Playing, Paused, Fade};

    class PlayModePanel : public QWidget
    {
        Q_OBJECT

    public:
        explicit PlayModePanel(QWidget *parent = nullptr);
        ~PlayModePanel();
        QPushButton* go_current_button();
        bool keep_current_checked();
        void update_ouput_status_label(OATS::PlayMode, const QString);
    signals:
        void go_current();
        void keep_current(bool);
    private slots:
        void go_current_clicked();
        void keep_current_clicked();
    private:
        void flash_status_label();
        void set_playing_status_style();
        void set_fading_status_style();

        std::unique_ptr<QLabel> m_play_mode_label;
        std::unique_ptr<QLabel> m_output_status_label;
        std::unique_ptr<QPushButton> m_btn_go_current;
        std::unique_ptr<QPushButton> m_btn_keep_current;
        QHBoxLayout* m_layout;
        bool m_stop_status_flash {false};
        std::unique_ptr<QTimer> m_status_flash_timer;
        std::unique_ptr<QTimer> m_fading_timer;
    };
}
