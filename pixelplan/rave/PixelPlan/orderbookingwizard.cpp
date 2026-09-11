#include "order.h"
#include "orderbookingwizard.h"

#include "bookingdatepage.h"
#include "bookingrulespage.h"
#include "buildbreakspage.h"
#include "breakselectionpage.h"
#include "summarypage.h"

namespace PIXELPLAN
{

    OrderBookingWizard::OrderBookingWizard(const QString& username,
                           std::shared_ptr<Order> order, QWidget* parent)
    : m_order{ order }
    {
        booking_data.order = order;
        booking_data.username = username;

        const int WIDTH = 865;
        const int HEIGHT = 600;

        setMinimumSize(WIDTH, HEIGHT);
        setWizardStyle(QWizard::ModernStyle);

        addPage(register_page<SpotsPage>(this));
        addPage(register_page<PIXELPLAN::BookingDatePage>(this));
        addPage(register_page<BookingRulesPage>(this));
        addPage(register_page<BuildBreaksPage>(this));
        addPage(register_page<BreakSelectionPage>(this));
        addPage(register_page<SummaryPage>(this));

        // booking_data.m_engine_data = std::make_unique<PIXELPLAN::EngineData>();
        // booking_data.m_rule_engine = std::make_unique<PIXELPLAN::RuleEngine>(*booking_data.m_engine_data);
    }

    OrderBookingWizard::~OrderBookingWizard()
    {
        m_pages.clear();
    }

    void OrderBookingWizard::init_rules_state() {
        auto brp = dynamic_cast<BookingRulesPage*>(m_pages[BookingRulesPage::name]);
        brp->init_rules_state();
    }

}