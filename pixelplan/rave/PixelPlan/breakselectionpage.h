#ifndef BREAKSELECTIONPAGE_H
#define BREAKSELECTIONPAGE_H

#include <QWizardPage>

class QLabel;
class QCheckBox;
class QTableWidget;
class QTableWidgetItem;

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
        bool validatePage() override;

    private slots:
        void on_state_changed(int);
        void on_item_clicked(QTableWidgetItem*);

    private:
        void setup_ui();
        void setup_break_select_grid();
        void show_selection_count();

        OrderBookingWizard* m_wizard;
        QTableWidget* m_tw_break_select;
        QLabel* m_lbl_avail_val;
        QLabel* m_lbl_sel_val;
        QLabel* m_lbl_order_val;
        QLabel* m_lbl_spots_ordered_val;
        QLabel* m_lbl_spots_booked_val;
        QCheckBox* m_cb_all;

    };
}

#endif