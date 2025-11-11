#ifndef BOOKINGWIZARD_H
#define BOOKINGWIZARD_H

#include <map>

#include <set>
#include <QWizard>
#include <QMenu>
#include <QJsonObject>

#include "../../../rave/framework/schedule.h"
#include "../../../rave/framework/entitydatamodel.h"

#include "timeband.h"
#include "traffikrules.h"
#include "spot.h"

class QTableWidgetItem;

namespace Ui {
    class BookingWizard;
}

class Order;
class EntityDataModel;
class DayPartGrid;
class Schedule;
class WizardData;
class RavenSetup;

class ToggleButton;
class JsonCacheHandler;

class BreakLayout;

struct SelectedBreak{
    int break_id;
    QDate break_date;
    QTime break_time;
    int break_hour;
    int booked_spots;
    int max_spots;
    std::string break_fill_method;
};

struct AllBreaks {
    int from_hour=0;
    int to_hour =0;
    std::vector<int> hourly_intervals;
};

struct ProgramBreak {
    int line_id = 0;
    QString break_time = "";
    int break_hour = 0;
    int duration   = 0;
    int max_spots  = 0;
    QString fill_method = "";
    QString dow = "";
    int progid = 0;
};

struct Program {
    QString title = "";
    std::vector<ProgramBreak> breaks;
};

enum class FillPos { First, Between, Last};

struct SelectedProgramBreak {
    QString break_time;
    int duration;
    int max_spots;
    FillPos fill_pos;
};

const QString rules_cache_file = "rules_cache_file.json";

using progid = int;

class BookingWizard : public QWizard
{
    Q_OBJECT

public:
    using Break = Schedule;

    enum {Page_Spots=0, Page_Dates, Page_Rules, Page_Build_Breaks,
          Page_Select_By_Day, Page_Select_By_Date, Page_Final};

    std::vector<std::string> days_of_week{"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

    explicit BookingWizard(Order* order,  QWidget* parent = nullptr);
    ~BookingWizard() override;

    std::size_t fetch_breaks_from_db(QDate, QDate, std::set<int>);
    std::size_t fetch_program_breaks_from_db(QDate, QDate, std::vector<SelectedProgramBreak>);

    void find_existing_bookings(TRAFFIK::EngineData&);
    std::size_t find_available_breaks();

    bool validateCurrentPage() override;
    int nextId() const override;
    void initializePage(int) override;

    TRAFFIK::Spot* selected_spot();

    void fetch_type_exclusions(TRAFFIK::EngineData&);
    void fetch_voice_exclusions(TRAFFIK::EngineData&);
    void fetch_spot_exclusions(const std::string,
                             std::vector<Exclusion>&,
                             std::list<int>& keys);
    int get_spot_audio(int);
    int get_spot_media(int);
    Daypart fetch_spot_daypart(TRAFFIK::Spot&);

    void init_rules_state();
    void show_available_breaks();
    std::vector<int> selected_hours(const std::string&);

    void test_booking();
    int find_break_slot(int, int);

    void set_toggle_buttons();

    void read_break_rules_cache();
    void cache_break_rules();
    void toggle_rule_buttons(QJsonObject);

    void reset_values();
    void color_label(QLabel*, Qt::GlobalColor);

    void test_set_start_end_dates();
    void add_tb_widget();

    void distribute_spot_to_available_breaks();

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
    // void timeBandChanged(int i);
    void all_breaks(bool);
    void tv_programs(bool);
    void toggleTimeBand(bool);
    void manual_time(bool);
    void build_breaks();
    void apply_selection();
    void clear_selection();
    void show_spot_details(const QPoint&);
    void show_breaks(int);
    //void time_band_selected(const QModelIndex&);
    void time_band_selected2(QTableWidgetItem*);
    void on_programs_clicked(QListWidgetItem*);
    void on_select_all_programs();
    void on_select_all_breaks();
    void on_all_break_by_date_selected(bool);

private:
    void print_selected_breaks();

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
    void auto_select_breaks(const AllBreaks&);
    int pick_random_interval();
    std::vector<int> get_hourly_distribution();

    bool spot_has_audio(const TRAFFIK::Spot*);
    void spot_details(int);

    void populate_from_to_combos(const std::set<int>&);
    void add_break_interval();

    std::map<progid, Program> get_tv_programs();
    // void fetch_program_breaks(std::string);

    std::vector<SelectedProgramBreak> get_selected_program_breaks();

    void set_breaks_table(int);
    int get_break_count(int);

    void show_tv_programs(std::map<progid, Program>);
    std::vector<ProgramBreak> fetch_all_breaks(std::map<progid, Program>);
    std::vector<int> get_program_ids(const std::map<progid, Program>&);
    void assign_breaks_to_programs(const std::vector<ProgramBreak>&, std::map<progid, Program>&);
    void show_program_breaks(std::vector<ProgramBreak>);

    Ui::BookingWizard *ui;
    Order* m_order;
    std::unique_ptr<EntityDataModel> m_spot_EDM;
    EntityDataModel* m_timeband_EDM;
    std::unique_ptr<DayPartGrid> m_daypart_grid;
    std::unique_ptr<EntityDataModel> m_booking_EDM;

    TRAFFIK::EngineData m_engine_data;
    std::unique_ptr<TRAFFIK::RuleEngine> m_rule_engine;

    std::map<int, std::vector<std::string>> m_dow_selection;

    ToggleButton* m_toggle_all;
    ToggleButton* m_remember_rules;

    ToggleButton* m_toggle_break_duration;
    ToggleButton* m_toggle_type_ex;
    ToggleButton* m_toggle_voice_ex;
    ToggleButton* m_toggle_type_daypart;
    ToggleButton* m_toggle_voice_daypart;
    ToggleButton* m_toggle_spot_daypart;
    ToggleButton* m_toggle_same_client;
    ToggleButton* m_toggle_override;

    std::vector<ToggleButton*> m_toggle_buttons;

    std::unique_ptr<JsonCacheHandler> m_cache_handler;
    QJsonObject m_rules_cache;

    std::shared_ptr<BreakLayout> m_break_layout;

    std::unique_ptr<EntityDataModel> m_edm_break_layout;
    std::unique_ptr<EntityDataModel> m_edm_breaks;


    /* --------------- private methods -------------------- */
    std::set<int> selected_unique_hours();
    std::set<int> select_unique_hours_from_breaks();

    QMenu* m_spot_ctx_menu;
    QAction* m_spot_ctx_action;

    std::map<int, SelectedBreak> m_selected_breaks;

    std::unique_ptr<EntityDataModel> m_edm_setup;
    std::shared_ptr<RavenSetup> m_raven_setup;

    std::map<progid, Program> m_programs;


};


#endif // BOOKINGWIZARD_H
