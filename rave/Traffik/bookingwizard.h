#ifndef BOOKINGWIZARD_H
#define BOOKINGWIZARD_H

#include <set>
#include <QWizard>
#include <QMenu>
#include "timeband.h"
#include "../framework/schedule.h"
#include "../framework/entitydatamodel.h"
#include "traffikrules.h"
#include "spot.h"

namespace Ui {
    class BookingWizard;
}

class Order;
class EntityDataModel;
class DayPartGrid;
class Schedule;
class WizardData;

struct SelectedBreak{
    int break_id;
    QDate break_date;
    QTime break_time;
    int break_hour;
};

class BookingWizard : public QWizard
{
    Q_OBJECT

public:
    using Break = Schedule;

    enum {Page_Spots=0, Page_Dates, Page_Rules, Page_Build_Breaks,
          Page_Select_By_Day, Page_Select_By_Date, Page_Final};

    std::vector<std::string> days_of_week{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    explicit BookingWizard(Order* order, QWidget* parent = nullptr);
    ~BookingWizard() override;

    std::size_t fetch_breaks(QDate, QDate, std::set<int>);
    void find_existing_bookings(TRAFFIK::EngineData&);
    std::size_t find_available_breaks();

    bool validateCurrentPage() override;
    TRAFFIK::Spot* selected_spot();

    void fetch_type_exclusions(TRAFFIK::EngineData&);
    void fetch_voice_exclusions(TRAFFIK::EngineData&);
    void fetch_spot_exclusions(const std::string,
                             std::vector<Exclusion>&,
                             std::list<int>& keys);
    Daypart fetch_spot_daypart(TRAFFIK::Spot&);

    void init_rules_state();
    void show_available_breaks();
    std::vector<int> selected_hours(const std::string&);

    void test_booking();


    int to_int(std::string s){
        return (s.empty()) ? 0 : std::stoi(s);
    }
    double to_double(std::string s){
        return (s.empty()) ? 0.0 : std::stod(s);
    }

    template<typename T1, typename T2>
    std::list<T1> keys(std::tuple<T1, T2>& m){
        std::list<T1> k;
        for (auto it=m.begin(); it != m.end(); ++it){
            k.push_back(it->first);
        }
        return k;
    }

private slots:
    void timeBandChanged(int i);
    void all_breaks(bool);
    void toggleTimeBand(bool);
    void build_breaks();
    void apply_selection();
    void clear_selection();
    void show_spot_details(const QPoint&);

private:
    Ui::BookingWizard *ui;
    Order* m_order;
    std::unique_ptr<EntityDataModel> m_spot_EDM;
    EntityDataModel* m_timeband_EDM;
    std::unique_ptr<DayPartGrid> m_daypart_grid;
    std::unique_ptr<EntityDataModel> m_booking_EDM;

    TRAFFIK::EngineData m_engine_data;
    std::unique_ptr<TRAFFIK::RuleEngine> m_rule_engine;

    std::map<int, std::vector<std::string>> m_dow_selection;

    std::set<int> selected_unique_hours();

    QMenu* m_spot_ctx_menu;
    QAction* m_spot_ctx_action;

    std::map<int, SelectedBreak> m_selected_breaks;

    void populate_spots_table(int);
    void populate_grid(TimeBand*);

    void setup_break_select_grid();
    bool make_booking();
    void commit_booking();
    void show_order_details(Order*);
    void add_days_of_week();
    void show_breaks_for_current_timeband();
    void toggle_selection(bool);

    void auto_select_breaks_by_dow();
    bool spot_has_audio(const TRAFFIK::Spot*);
    void spot_details(int);

};


#endif // BOOKINGWIZARD_H
