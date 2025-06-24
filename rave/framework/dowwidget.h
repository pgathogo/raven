#ifndef DOWWIDGET_H
#define DOWWIDGET_H

#include <map>
#include <memory>

#include <QWidget>
#include <QSpinBox>
#include <QGridLayout>


class DowWidget : public QWidget
{
    Q_OBJECT

public:
    DowWidget(QWidget* parent = nullptr);

    void make_week_days();
    void make_dow_grid();
    QGridLayout* grid_layout();

private:
    std::map<int, std::unique_ptr<QSpinBox>> m_dow;
    std::map<int, QString> m_week_days;
    QGridLayout* m_glDow;
};


#endif
