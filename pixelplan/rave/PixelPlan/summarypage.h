#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <QWizardPage>

namespace PIXELPLAN
{
    class OrderBookingWizard;

    class SummaryPage: public QWizardPage
    {
        Q_OBJECT
    public:
        SummaryPage(OrderBookingWizard*);

        QString name{"summary_page"};

    private:
        void setup_ui();

        OrderBookingWizard* m_wizard;

    };
}

#endif