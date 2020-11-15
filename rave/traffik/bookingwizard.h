#ifndef BOOKINGWIZARD_H
#define BOOKINGWIZARD_H

#include <QWizard>
#include "../framework/entitydatamodel.h"
#include "timeband.h"
#include "schedule.h"
#include "traffikrules.h"

namespace Ui {
    class BookingWizard;
}

class Order;
class Spot;
class EntityDataModel;
class DayPartGrid;
class Schedule;
class WizardData;


class BookingWizard : public QWizard
{
    Q_OBJECT

public:
    using Break = Schedule;

    explicit BookingWizard(Order* order, QWidget* parent = nullptr);
    ~BookingWizard();

    void populate_spots_table(int client_id);
    void populate_grid(TimeBand*);
    std::size_t fetch_breaks(QDate, QDate);
    void find_existing_bookings(TRAFFIK::EngineData&);
    int find_available_breaks();

    bool validateCurrentPage() override;
    Spot* selected_spot();

    void fetch_type_exclusions(int);
    void fetch_voice_exclusions(int);
    void fetch_spot_exclusions(const std::string,
                             std::vector<Exclusion>&,
                             std::list<int>& keys);

    void init_rules_state();
    void available_breaks_summary();

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
    void allBreaks(bool);
    void toggleTimeBand(bool);
    void buildBreaks();
    void breakDuration(int state);

private:
    Ui::BookingWizard *ui;
    Order* m_order;
    std::unique_ptr<EntityDataModel> m_spot_EDM;
    EntityDataModel* m_timeband_EDM;
    std::unique_ptr<DayPartGrid> m_daypart_grid;
    std::unique_ptr<EntityDataModel> m_schedule_EDM;
    std::unique_ptr<EntityDataModel> m_booking_EDM;

    TRAFFIK::EngineData m_engine_data;

    std::unique_ptr<TRAFFIK::RuleEngine> m_rule_engine;

};




#endif // BOOKINGWIZARD_H
