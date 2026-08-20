#ifndef BREAKSELECTIONPAGE_H
#define BREAKSELECTIONPAGE_H

#include <QWizardPage>

namespace PIXELPLAN
{
    class OrderBookingWizard;

    class BreakSelectionPage : public QWizardPage
    {
        Q_OBJECT
    public:
        BreakSelectionPage(OrderBookingWizard*);
        QString name{"break_selection_page"};

        void initializePage() override;

    private:
        void setup_ui();
        void setup_break_select_grid();

        OrderBookingWizard* m_wizard;
        QTableWidget* m_tw_break_select;

    };
}

#endif