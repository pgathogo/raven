#ifndef SUMMARYPAGE_H
#define SUMMARYPAGE_H

#include <QWizardPage>

class QTreeView;

namespace PIXELPLAN
{
    class OrderBookingWizard;

    class SummaryPage: public QWizardPage
    {
        Q_OBJECT
    public:
        SummaryPage(OrderBookingWizard*);

        void initializePage() override;
        bool validatePage() override;

        QString name{"summary_page"};

    private:
        void setup_ui();
        bool make_booking();
        void commit_booking();
        int find_break_slot(int, int);

        QTreeView* m_tv_summary;
        OrderBookingWizard* m_wizard;

    };
}

#endif