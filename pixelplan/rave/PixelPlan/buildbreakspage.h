#ifndef BUILDBREAKSPAGE_H
#define BUILDBREAKSPAGE_H

#include <functional>
#include <QWizardPage>
#include <QString>
#include "traffikrules.h"

class QLabel;

namespace PIXELPLAN
{
class OrderBookingWizard;

    struct Collision {
        QString name;
        QString desc;
        std::function<int(void)> break_count;
        QLabel* label;
        QLabel* image_label;
    };

    class BuildBreaksPage : public QWizardPage
    {
        Q_OBJECT
    public:
        BuildBreaksPage(OrderBookingWizard*);

        QString name{"build_breaks_page"};
    private slots:
        void on_build_breaks();

    private:
        void setup_ui();
        void reset_values();
        void init_collisions_data();
        void color_label(QLabel*, Qt::GlobalColor);
        void fetch_type_exclusions(PIXELPLAN::EngineData&);
        Daypart fetch_spot_daypart(TRAFFIK::Spot&);
        std::size_t fetch_breaks_from_db(QDate, QDate, std::set<int>);
        std::size_t fetch_program_breaks_from_db(QDate, QDate, std::vector<SelectedProgramBreak>);
        void fetch_voice_exclusions(PIXELPLAN::EngineData&);
        void fetch_spot_exclusions(const std::string,
                       std::vector<Exclusion>&,
                       std::list<int>&);
        int get_spot_media(int);
        void find_existing_bookings(PIXELPLAN::EngineData&);
        std::size_t find_available_breaks();
        void show_available_breaks();
        std::vector<int> selected_hours(const std::string&);

        // std::shared_ptr<BookingContext> m_booking_context;
        OrderBookingWizard* m_wizard;

        std::shared_ptr<PIXELPLAN::RuleEngine> m_rule_engine;
        std::shared_ptr<PIXELPLAN::EngineData> m_engine_data;

        std::vector<Collision> m_collisions;

        QLabel* m_lbl_bchecked_value;
        QLabel* m_lbl_total_val;
    };


}

#endif