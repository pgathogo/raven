#ifndef BOOKINGRULESPAGE_H
#define BOOKINGRULESPAGE_H

#include <vector>
#include <QWizardPage>
#include <QString>

class ToggleButton;

namespace PIXELPLAN
{
   class OrderBookingWizard;
    const int RULE_COUNT = 8;

    struct RuleConfig {
        QString name;
        QString desc;
        ToggleButton* btn;
    };

    class BookingRulesPage : public QWizardPage
    {
        Q_OBJECT
    public:
        BookingRulesPage(OrderBookingWizard*);
        void init_rules_state();

        inline static QString name{"booking_rules_page"};

    private:
        void setup_ui();
        void set_rule_config();

        // std::shared_ptr<BookingContext> m_booking_context;
        OrderBookingWizard* m_wizard;
        std::vector<RuleConfig> m_rules_config;
    };
}

#endif