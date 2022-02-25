#pragma once

#include <memory>

#include <QWidget>
#include <QHBoxLayout>

class QLabel;
class QPushButton;

namespace OATS{

    class PlayModePanel : public QWidget
    {
        Q_OBJECT
    public:
        explicit PlayModePanel(QWidget *parent = nullptr);
        ~PlayModePanel();

    signals:
        void go_current();
        void keep_current();
    private slots:
        void go_current_clicked();
        void keep_current_clicked();
    private:
        std::unique_ptr<QLabel> m_play_mode_label;
        std::unique_ptr<QPushButton> m_btn_go_current;
        std::unique_ptr<QPushButton> m_btn_keep_current;
        QHBoxLayout* m_layout;
    };
}
