#pragma once

#include <QWidget>

class QHBoxLayout;
class QLabel;

namespace OATS
{

    class TimeAnalyzerTextWidget : public QWidget
    {
        Q_OBJECT
    public:
        TimeAnalyzerTextWidget();
        ~TimeAnalyzerTextWidget();

        void make_widget();
        void update_stats(std::tuple<QString, QString, QString>);

    private:
        QHBoxLayout* m_main_layout;

        QLabel* m_lbl_music_remain;
        QLabel* m_lbl_hour_remain;
        QLabel* m_lbl_talk_remain;

    };

}
