#include <QPainter>
#include <QVBoxLayout>
#include <QGraphicsLayout>

#include "timeanalyzerwidget.h"

TimeAnalyzerWidget::TimeAnalyzerWidget()
    :m_time_bar{nullptr}
    ,m_series{nullptr}
{
    m_time_bar = std::make_unique<QtCharts::QBarSet>("Hour Remaining");
    m_music_bar = std::make_unique<QtCharts::QBarSet>("Music Remaining");
    m_talk_bar = std::make_unique<QtCharts::QBarSet>("Talk Time");

    m_series = std::make_unique<QtCharts::QHorizontalStackedBarSeries>();
    m_series->setLabelsVisible(true);

    m_chart = std::make_unique<QtCharts::QChart>();
    m_chart->setMargins(QMargins(0,0,0,0));
    m_chart_view = std::make_unique<QtCharts::QChartView>(m_chart.get());
    m_chart_view->setRenderHint(QPainter::Antialiasing);

    QFont chart_font("DigifaceWide", 12, QFont::Bold);
//    m_chart->setFont(chart_font);

    m_chart_view->setFont(chart_font);

    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_chart_view.get());
    this->setLayout(m_layout);
}

TimeAnalyzerWidget::~TimeAnalyzerWidget()
{
    delete m_layout;
}


void TimeAnalyzerWidget::set_time_bar(std::map<QString, float> time_slots)
{
    *m_time_bar << time_slots["time_remain"];
    *m_music_bar << time_slots["music_remain"];
    *m_talk_bar << time_slots["talk_time"];

    m_series->clear();
    m_series->append(m_time_bar.get());
    m_series->append(m_music_bar.get());
    m_series->append(m_talk_bar.get());

    m_chart->addSeries(m_series.get());
    m_chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignRight);

}
