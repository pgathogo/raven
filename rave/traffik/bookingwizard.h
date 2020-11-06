#ifndef BOOKINGWIZARD_H
#define BOOKINGWIZARD_H

#include <QWizard>
#include "../framework/entitydatamodel.h"
#include "timeband.h"

namespace Ui {
class BookingWizard;
}

class Order;
class Spot;
class EntityDataModel;
class DayPartGrid;
class Schedule;

enum FailedBreakCode {BreakFull=1, TypeExcl, VoiceExcl,
                    TypeDaypart, VoiceDaypart, SpotDaypart, SameClient};

using ScheduleId = int;
using FailedBreak = std::tuple<ScheduleId, FailedBreakCode>;

class BookingWizard : public QWizard
{
    Q_OBJECT

public:
    using ExclusionID = int;
    using DayOfWeek = int;
    using Daypart = std::map<DayOfWeek, std::string>;
    using Exclusion = std::tuple<ExclusionID, Daypart>;
    using Break = Schedule;

    explicit BookingWizard(Order* order, QWidget* parent = nullptr);
    ~BookingWizard();

    void populateSpotsTable(int client_id);
    void populateGrid(TimeBand*);
    std::size_t fetchBreaks(QDate, QDate);
    void findExistingBookings();
    int findAvailableBreaks();

    bool validateCurrentPage() override;
    Spot* selectedSpot();

    void fetchTypeExclusions(int);
    void fetchVoiceExclusions(int);
    void fetchSpotExclusions(const std::string, std::vector<Exclusion>&, std::list<int> keys);
    void print_exclusions(std::vector<Exclusion>&);
    void log_failed_break(Break*, FailedBreakCode);

    //bool check_collusion(type_exclusion);

    struct SpotRecord{
        int spot_id;
        std::string spot_name;
        double spot_duration;
        double real_duration;
        Daypart spot_daypart;
        std::list<int> type_ex_keys;
        std::list<int> voice_ex_keys;
        std::vector<Exclusion> type_exclusions;
        std::vector<Exclusion> voice_exclusions;
    };

    struct BookingRecord{
        int booking_id;
        int schedule_id;
        int bookingsegment_id;
        int brand_id;
        int client_id;
        SpotRecord spotRecord;
    };

    struct WizardData{
        std::size_t break_count = 0;
        int full_breaks = 0;
        int type_excl_collusion = 0;
        SpotRecord current_spot;
        std::vector<BookingRecord> prev_bookings;
        std::vector<FailedBreak> failed_breaks;

        void log_failed_breaks(Schedule* sched, FailedBreakCode fbc){

        }
    };

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

private:
    Ui::BookingWizard *ui;
    Order* mOrder;
    std::unique_ptr<EntityDataModel> spotEDM;
    EntityDataModel* timeBandEDM;
    std::unique_ptr<DayPartGrid> mDPG;
    std::unique_ptr<EntityDataModel> scheduleEDM;
    std::unique_ptr<EntityDataModel> bookingEDM;

    WizardData wizardData;

};

#endif // BOOKINGWIZARD_H
