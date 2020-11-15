#ifndef TRAFFIKRULES_H
#define TRAFFIKRULES_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>

class QCheckBox;
class Schedule;

using ExclusionID = int;
using DayOfWeek = int;
using Daypart = std::map<DayOfWeek, std::string>;
using Exclusion = std::tuple<ExclusionID, Daypart>;

namespace TRAFFIK {

    enum FailedBreakCode {BreakFull=1, TypeExcl, VoiceExcl,
                        TypeDaypart, VoiceDaypart, SpotDaypart, SameClient};

    enum RuleState {Disabled=0, Enabled=2};

    using ScheduleId = int;
    using Break = Schedule;
    using FailedBreak = std::tuple<ScheduleId, FailedBreakCode>;

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
        SpotRecord booked_spot;
    };

    struct EngineData{
        std::size_t break_count = 0;
        int full_breaks = 0;
        int type_excl_collusion = 0;
        SpotRecord spot_to_book;
        BookingRecord booked_record;
        std::vector<BookingRecord> prev_bookings;
        std::vector<FailedBreak> failed_breaks;
        std::map<int , std::string> target_daypart;
    };

    class BaseRule;

    class RuleEngine{
    public:
        RuleEngine(EngineData& wd);
        bool run(Break* target_break);

        bool find_failed_break(std::vector<FailedBreak>& fb, int break_id);

        template<typename T>
        void register_rule(){
            auto t = new T();
            m_rules.push_back(t);
        }

    private:
        EngineData& mEngineData;
        std::vector<BaseRule*> m_rules;
    };

    using BookedRecord = BookingRecord;

    class BaseRule{
    public:
        virtual ~BaseRule();
        virtual bool execute(EngineData& engine_data,
                             [[maybe_unused]] const BookedRecord& booked_record,
                             Break& target_break) = 0;
        virtual bool isEnabled() = 0;
        virtual FailedBreakCode fail_code() = 0;
        virtual std::string rule_name() = 0;
    };

    class FullBreakRule : public BaseRule{
    public:
        FullBreakRule();
        ~FullBreakRule() override;
        bool execute(EngineData& engine_data,
                     [[maybe_unused]] const BookedRecord& booked_record,
                     Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "FullBreakRule"; }
        static void enable_or_disable(int state);
        static int failed_break_count();
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    class TypeExclusionRule : public BaseRule {
    public:
        TypeExclusionRule();
        ~TypeExclusionRule() override;
        bool execute(EngineData& engine_data,
                     const BookedRecord& booked_record,
                     Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "TypeExclusionRule"; }
        static void enable_or_disable(int state);
        static int failed_break_count();
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    class VoiceExclusionRule : public BaseRule{
    public:
        VoiceExclusionRule();
        ~VoiceExclusionRule() override;
        bool execute(EngineData& engine_data,
                     const BookedRecord& booked_record,
                     Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "VoiceExclusionRule"; }
        static void enable_or_disable(int state);
        static int failed_break_count();
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    //Type Daypart collusions
    class TypeDaypartRule : public BaseRule{
    public:
        TypeDaypartRule();
        ~TypeDaypartRule() override;
        bool execute(EngineData& engine_data,
                     const BookedRecord& booked_record,
                     Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "TypeDaypartRule"; }
        static void enable_or_disable(int state);
        static int failed_break_count();
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

}

#endif // TRAFFIKRULES_H
