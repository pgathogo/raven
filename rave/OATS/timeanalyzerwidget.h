#pragma once

#include <memory>
#include <QWidget>

#include <QtCharts/QBarSet>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

class QVBoxLayout;

//namespace QtCharts{
//    class QBarSet;
//    class QHorizontalStackedBarSeries;
//    class QChartView;
//    class QChart;
//    class QChartView;
//}

class TimeAnalyzerWidget : public QWidget
{
public:
    TimeAnalyzerWidget();
    ~TimeAnalyzerWidget();
    void set_time_bar(std::map<QString, float>);
//    QtCharts::QChartView* time_chart();
private:
    std::unique_ptr<QtCharts::QBarSet> m_time_bar;
    std::unique_ptr<QtCharts::QBarSet> m_music_bar;
    std::unique_ptr<QtCharts::QBarSet> m_talk_bar;

    std::unique_ptr<QtCharts::QHorizontalStackedBarSeries> m_series;
    std::unique_ptr<QtCharts::QChart> m_chart;
    std::unique_ptr<QtCharts::QChartView> m_chart_view;
    QVBoxLayout* m_layout;
};

