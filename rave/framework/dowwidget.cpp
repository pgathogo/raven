#include <QLabel>

#include "dowwidget.h"

DowWidget::DowWidget(QWidget* parent)
{
   m_glDow = new QGridLayout();

    make_week_days();
    make_dow_grid();

    // QVBoxLayout* main_layout = new QVBoxLayout();
    // main_layout->addLayout(m_glDow);
     setLayout(m_glDow);



}

void DowWidget::make_week_days()
{
    m_week_days[0] = "Mon";
    m_week_days[1] = "Tue";
    m_week_days[2] = "Wed";
    m_week_days[3] = "Thr";
    m_week_days[4] = "Fri";
    m_week_days[5] = "Sat";
    m_week_days[6] = "Sun";
}

void DowWidget::make_dow_grid()
{

    int DOW  = 7;
    int ROW0 = 0;
    int ROW1 = 1;

    for(int day=0; day < DOW; ++day) {
        QLabel* lbl = new QLabel(m_week_days[day]);
        auto sp = std::make_unique<QSpinBox>();

        sp->setMinimumSize(50, 50);  //setMaximumWidth(50);
        sp->setRange(0, 99);

        m_glDow->addWidget(lbl, ROW0, day);
        m_glDow->addWidget(sp.get(), ROW1, day);

        m_dow[day+1] = std::move(sp);
    }

}

QGridLayout* DowWidget::grid_layout()
{
    return m_glDow;
}
