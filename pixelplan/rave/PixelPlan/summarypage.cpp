#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

#include "summarypage.h"

namespace PIXELPLAN
{

SummaryPage::SummaryPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Summary");
    setSubTitle("Verify break selection is okay before confirming the booking");

    setup_ui();
}

void SummaryPage::setup_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout();

    QTreeView* tv_summary = new QTreeView();

    QHBoxLayout* hl_summary = new QHBoxLayout();
    QLabel* lbl_total = new QLabel("Total Bookings:");
    QLabel* lbl_total_val = new QLabel("0");
    hl_summary->addWidget(lbl_total);
    hl_summary->addWidget(lbl_total_val);
    hl_summary->addStretch();

    main_layout->addWidget(tv_summary);
    main_layout->addLayout(hl_summary);

    setLayout(main_layout);

}

} // namespace