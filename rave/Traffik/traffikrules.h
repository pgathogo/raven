#ifndef TRAFFIKRULES_H
#define TRAFFIKRULES_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>

class QCheckBox;
class Schedule;
class EntityDataModel;
class TimeBand;

using ExclusionID = int;
using DayOfWeek = int;
//using Daypart = std::map<DayOfWeek, std::string>;
using Daypart = std::map<DayOfWeek, std::tuple<std::string, std::vector<int>>>;
using Exclusion = std::tuple<ExclusionID, Daypart>;


namespace TRAFFIK {

    enum FailedBreakCode {BreakFull=1, TypeExcl, VoiceExcl,
                        TypeDaypart, VoiceDaypart, SpotDaypart, SameClient};

    enum RuleState {Disabled=0, Enabled=2};

    using ScheduleId = int;
    using Break = Schedule;
    using FailedBreak = std::tuple<ScheduleId, FailedBreakCode>;

    struct SpotRecord{
        int spot_id{-1};
        int client_id{-1};
        int brand_id{-1};
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
        int booking_id{-1};
        int schedule_id{-1};
        int bookingsegment_id{-1};
        int book_seq{-1};
        SpotRecord booked_spot;
    };

    struct EngineData{
        EngineData();
        std::size_t break_count{0} ;
        std::size_t available_breaks{0};
        int full_breaks{0};
        int type_excl_collusion{0};
        SpotRecord spot_to_book;
        BookingRecord booked_record;
        TimeBand* current_time_band{nullptr};
        std::vector<BookingRecord> prev_bookings;
        std::vector<FailedBreak> failed_breaks;
        std::unique_ptr<EntityDataModel> m_schedule_EDM;
        std::map<int , std::tuple<std::string, std::vector<int>>> target_daypart;
    };

    class BaseRule;

    class RuleEngine{
    public:
        RuleEngine(EngineData& wd);

        int find_breaks();

        template<typename T>
        void register_rule(){
            auto t = new T();
            m_rules.push_back(t);
        }

    private:
        EngineData& m_engine_data;
        std::vector<BaseRule*> m_rules;

        bool find_failed_break(std::vector<FailedBreak>& fb, int break_id);
        bool check_break_availability(Break* target_break);
        void log_failure(int break_id, FailedBreakCode fbc);
    };

    using BookedRecord = BookingRecord;

    class BaseRule{
    public:
        virtual ~BaseRule();
        virtual bool execute(EngineData& engine_data,
                             const Break& target_break) = 0;
        virtual bool isEnabled() = 0;
        virtual FailedBreakCode fail_code() = 0;
        virtual std::string rule_name() = 0;
        virtual void reset_break_count() = 0;
    };

    class FullBreakRule : public BaseRule{
    public:
        FullBreakRule();
        ~FullBreakRule() override;
        bool execute(EngineData& engine_data,
                     const Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "FullBreakRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void test_invoke();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    class TypeExclusionRule : public BaseRule {
    public:
        TypeExclusionRule();
        ~TypeExclusionRule() override;
        bool execute(EngineData& engine_data,
                     const Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "TypeExclusionRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    class VoiceExclusionRule : public BaseRule{
    public:
        VoiceExclusionRule();
        ~VoiceExclusionRule() override;
        bool execute(EngineData& engine_data,
                     const Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "VoiceExclusionRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
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
                    const  Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "TypeDaypartRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        std::map<int, bool> cache_hour{};
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    //Type Daypart collusions
    class VoiceDaypartRule : public BaseRule{
    public:
        VoiceDaypartRule();
        ~VoiceDaypartRule() override;
        bool execute(EngineData& engine_data,
                    const  Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "VoiceDaypartRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        std::map<int, bool> cache_hour{};
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    //Spot Daypart
    class SpotDaypartRule : public BaseRule{
    public:
        SpotDaypartRule();
        ~SpotDaypartRule() override;
        bool execute(EngineData& engine_data,
                    const  Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "SpotDaypartRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        std::map<int, bool> cache_hour{};
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    /* SameClientRule */
    class SameClientRule : public BaseRule {
    public:
        SameClientRule();
        ~SameClientRule() override;
        bool execute(EngineData& engine_data,
                     const Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "SameClientRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

    /* OverrideSameClientRule */
    class OverrideSameClientRule : public BaseRule {
    public:
        OverrideSameClientRule();
        ~OverrideSameClientRule() override;
        bool execute(EngineData& engine_data,
                     const Break& target_break) override;
        bool isEnabled() override;
        FailedBreakCode fail_code() override;
        std::string rule_name() override { return "OverrideSameClientRule"; }
        static void enable_or_disable(bool state);
        static int failed_break_count();
        void reset_break_count() override { m_failed_break_count = 0; }
    private:
        static bool m_isEnabled;
        static int m_failed_break_count;
    };

}

#endif // TRAFFIKRULES_H
