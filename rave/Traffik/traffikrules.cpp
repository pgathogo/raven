#include <algorithm>
#include <QCheckBox>
#include "traffikrules.h"
#include "../framework/schedule.h"
#include "../framework/entitydatamodel.h"

#define DEBUG_MODE

namespace TRAFFIK{

    EngineData::EngineData()
    {
        m_schedule_EDM = std::make_unique<EntityDataModel>(
                    std::make_unique<Schedule>());
    }

    RuleEngine::RuleEngine(EngineData &ed)
        :m_engine_data{ed}
    {
        register_rule<FullBreakRule>();
        register_rule<TypeExclusionRule>();
        register_rule<VoiceExclusionRule>();
        register_rule<TypeDaypartRule>();
        register_rule<VoiceDaypartRule>();
        register_rule<SpotDaypartRule>();
        register_rule<SameClientRule>();
        register_rule<BlockDiffSpotSameAudioRule>();
    }

    int RuleEngine::find_breaks()
    {
        m_engine_data.failed_breaks.clear();

        // Initialize break failed counts for all the rules
        for (auto& rule : m_rules )
            rule->reset_break_count();

        for (auto& [name, entity] : m_engine_data.m_schedule_EDM->modelEntities())
        {
            auto target_break = dynamic_cast<Break*>(entity.get());
            check_break_availability(target_break);
        }

        // Override same client rule for different
        OverrideSameClientRule osc_rule;
        if (osc_rule.isEnabled())
             osc_rule.execute(m_engine_data, Break());

        return  ( m_engine_data.break_count - m_engine_data.failed_breaks.size() );
    }

    bool RuleEngine::check_break_availability(Break* target_break)
    {
        /*
          - Breaks Checked

          ***[ Rules depend with the prev booking ]***
          - Type Exclusion collusions
          - Voice Exclusion collusions
          - Same Client collusions
          - Override Same Client for Different brands **

          ***[ Rules don't depend with the prev booking ]***
          - Full Breaks
          - Type Daypart collusions
          - Voice Daypart collusions
          - Spot Daypart collusions

          */

        for (auto& rule : m_rules )
        {
            if (!rule->isEnabled())
                continue;

            // If target break is logged as failed, skip the break
            if (find_failed_break(m_engine_data.failed_breaks, target_break->id())){
                return false;
            }

            bool result = rule->execute(m_engine_data, *target_break);

            if (!result)
            {
                log_failure(target_break->id(), rule->fail_code());
                target_break->set_break_availability(Schedule::BreakAvailability::Break_Not_Available);

            }

        }

        return true;
    }

    void RuleEngine::log_failure(int break_id, FailedBreakCode fbc)
    {
        m_engine_data.failed_breaks.push_back(std::make_tuple(break_id, fbc));
    }

    bool RuleEngine::find_failed_break(std::vector<FailedBreak>& failed_break, int break_id)
    {
        return (std::find_if(failed_break.begin(), failed_break.end(),
                      [&break_id](FailedBreak& fb){
                    return (std::get<0>(fb) == break_id) ? true : false; })
                != failed_break.end()) ? true : false;
    }


    BaseRule::~BaseRule()
    {

    }


    bool FullBreakRule::m_isEnabled = false;
    int FullBreakRule::m_failed_break_count = 0;

    FullBreakRule::FullBreakRule()
    {
    }

    FullBreakRule::~FullBreakRule()
    {
    }

    int FullBreakRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    void FullBreakRule::test_invoke()
    {
        printstr("test_invoke::Invoked");
    }

    bool FullBreakRule::execute(EngineData& engine_data,
                                    const Break& target_break)
    {
        if (target_break.break_duration_left()->value() <
                engine_data.spot_to_book.spot_duration)
        {
            ++m_failed_break_count;
            return false;
        }

        return true;
    }

    bool FullBreakRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode FullBreakRule::fail_code()
    {
        return FailedBreakCode::BreakFull;
    }

    void FullBreakRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;

    }

    bool TypeExclusionRule::m_isEnabled = false;
    int TypeExclusionRule::m_failed_break_count = 0;

    TypeExclusionRule::TypeExclusionRule()
    {
    }

    TypeExclusionRule::~TypeExclusionRule()
    {
    }

    bool TypeExclusionRule::execute(EngineData& engine_data,
                                    const Break& target_break)
    {
        for (const int& k : engine_data.spot_to_book.type_ex_keys)
        {
            for (auto& booked_record: engine_data.prev_bookings)
            {
                if (booked_record.schedule_id != target_break.id())
                    continue;

                if ((std::find(booked_record.booked_spot.type_ex_keys.begin(),
                             booked_record.booked_spot.type_ex_keys.end(), k) !=
                     booked_record.booked_spot.type_ex_keys.end()))
                {
                    ++m_failed_break_count;
                    return false;
                }
            }
        }

        return true;
    }

    bool TypeExclusionRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode TypeExclusionRule::fail_code()
    {
        return FailedBreakCode::TypeExcl;
    }

    void TypeExclusionRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;

    }

    int TypeExclusionRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    bool VoiceExclusionRule::m_isEnabled = false;
    int VoiceExclusionRule::m_failed_break_count = 0;

    VoiceExclusionRule::VoiceExclusionRule()
    {
    }

    VoiceExclusionRule::~VoiceExclusionRule()
    {
    }

    bool VoiceExclusionRule::execute(EngineData& engine_data,
                                     const Break& target_break)
    {
        for (const int& k : engine_data.spot_to_book.voice_ex_keys)
        {
            for (auto& booked_record: engine_data.prev_bookings)
            {
                if (booked_record.schedule_id != target_break.id())
                    continue;

                if ((std::find(booked_record.booked_spot.voice_ex_keys.begin(),
                             booked_record.booked_spot.voice_ex_keys.end(), k) !=
                     booked_record.booked_spot.voice_ex_keys.end()))
                {
                    ++m_failed_break_count;
                    return false;
                }
            }
        }

        return true;
    }

    bool VoiceExclusionRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode VoiceExclusionRule::fail_code()
    {
        return FailedBreakCode::VoiceExcl;
    }

    void VoiceExclusionRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;

    }

    int VoiceExclusionRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    /* ---------------------------------------------- */

    bool TypeDaypartRule::m_isEnabled = false;
    int TypeDaypartRule::m_failed_break_count = 0;

    TypeDaypartRule::TypeDaypartRule()
    {
    }

    TypeDaypartRule::~TypeDaypartRule()
    {
    }

    bool TypeDaypartRule::execute(EngineData& engine_data,
                                  const Break& target_break)
    {
        bool is_break_valid = true;

        auto target_break_hour = target_break.schedule_hour()->value();

        if (m_cache_hour.find(target_break_hour) != m_cache_hour.end())
        {
            if (m_cache_hour[target_break_hour] == false)
            {
                ++m_failed_break_count;
                return false;
            }
        }

        m_cache_hour.clear();

        int dow = target_break.schedule_date()->value().dayOfWeek();

        qDebug() << "Target Hour...: "<< target_break_hour;

        for (auto& type_excl : engine_data.spot_to_book.type_exclusions)
        {
            auto& [tid, daypart] = type_excl;
            auto& [dp_str, hours] = daypart[dow];


            qDebug() << "------------------------------------";
            std::string h_str;
            for(auto& h:hours){
                h_str = h_str + std::to_string(h)+", ";
            }
            qDebug() << stoq(h_str);
            qDebug() << "------------------------------------";

            if ( std::find(hours.begin(), hours.end(), target_break_hour) == hours.end() )
            {
                ++m_failed_break_count;
                is_break_valid = false;
            } else {
                is_break_valid = true;
            }
        }

        m_cache_hour[target_break_hour] = is_break_valid;

        return is_break_valid;

    }

    bool TypeDaypartRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode TypeDaypartRule::fail_code()
    {

        return FailedBreakCode::TypeDaypart;
    }

    void TypeDaypartRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;
    }

    int TypeDaypartRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    /* ---------------------------------------------- */

    bool VoiceDaypartRule::m_isEnabled = false;
    int VoiceDaypartRule::m_failed_break_count = 0;

    VoiceDaypartRule::VoiceDaypartRule()
    {
    }

    VoiceDaypartRule::~VoiceDaypartRule()
    {
    }

    bool VoiceDaypartRule::execute(EngineData& engine_data,
                                  const Break& target_break)
    {
        bool is_break_valid = true;

        auto target_break_hour = target_break.schedule_hour()->value();

        if (cache_hour.find(target_break_hour) != cache_hour.end())
        {
            if (cache_hour[target_break_hour] == false)
            {
                ++m_failed_break_count;
                return false;
            }
        }

        cache_hour.clear();

        int dow = target_break.schedule_date()->value().dayOfWeek();

        for (auto& voice_excl : engine_data.spot_to_book.voice_exclusions)
        {
            auto& [tid, daypart] = voice_excl;
            auto& [dp_str, hours] = daypart[dow];

            if ( std::find(hours.begin(), hours.end(), target_break_hour) == hours.end() )
            {
                ++m_failed_break_count;
                is_break_valid = false;
            } else {
                is_break_valid = true;
            }
        }

        cache_hour[target_break_hour] = is_break_valid;

        return is_break_valid;

    }

    bool VoiceDaypartRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode VoiceDaypartRule::fail_code()
    {

        return FailedBreakCode::VoiceDaypart;
    }

    void VoiceDaypartRule::enable_or_disable(bool state)
    {
        m_isEnabled	= state;
    }

    int VoiceDaypartRule::failed_break_count()
    {
        return m_failed_break_count;
    }


    /* ---------------------------------------------- */

    bool SpotDaypartRule::m_isEnabled = false;
    int SpotDaypartRule::m_failed_break_count = 0;

    SpotDaypartRule::SpotDaypartRule()
    {
    }

    SpotDaypartRule::~SpotDaypartRule()
    {
    }

    bool SpotDaypartRule::execute(EngineData& engine_data,
                                  const Break& target_break)
    {
        bool is_break_valid = true;

        auto target_break_hour = target_break.schedule_hour()->value();

        if (cache_hour.find(target_break_hour) != cache_hour.end())
        {
            if (cache_hour[target_break_hour] == false)
            {
                ++m_failed_break_count;
                return false;
            }
        }

        cache_hour.clear();

        int dow = target_break.schedule_date()->value().dayOfWeek();

        auto& [dp_str, hours] = engine_data.spot_to_book.spot_daypart[dow];

        if ( std::find(hours.begin(), hours.end(), target_break_hour) == hours.end() )
        {
          ++m_failed_break_count;
          is_break_valid = false;
        } else {
           is_break_valid = true;
        }

        cache_hour[target_break_hour] = is_break_valid;

        return is_break_valid;

    }

    bool SpotDaypartRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode SpotDaypartRule::fail_code()
    {

        return FailedBreakCode::SpotDaypart;
    }

    void SpotDaypartRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;
    }

    int SpotDaypartRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    /* --- SameClientRule --- */

    bool SameClientRule::m_isEnabled = false;
    int SameClientRule::m_failed_break_count = 0;

    SameClientRule::SameClientRule()
    {
    }

    SameClientRule::~SameClientRule()
    {
    }

    bool SameClientRule::execute(EngineData& engine_data,
                                    const Break& target_break)
    {
        for(auto& booked_record: engine_data.prev_bookings)
        {
            if (booked_record.schedule_id != target_break.id())
                continue;

            if ( (booked_record.booked_spot.client_id == engine_data.spot_to_book.client_id ) &&
                 (booked_record.schedule_id == target_break.id()) )
            {
                ++m_failed_break_count;
                return false;
            }
        }

        return true;
    }

    bool SameClientRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode SameClientRule::fail_code()
    {
        return FailedBreakCode::SameClient;
    }

    void SameClientRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;
    }

    int SameClientRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    /* --- OverrideSameClientRule --- */

    bool OverrideSameClientRule::m_isEnabled = false;
    int OverrideSameClientRule::m_failed_break_count = 0;

    OverrideSameClientRule::OverrideSameClientRule()
    {
    }

    OverrideSameClientRule::~OverrideSameClientRule()
    {
    }

    bool OverrideSameClientRule::execute(EngineData& engine_data,
                                    [[maybe_unused]] const Break& target_break)
    {
        std::vector<int> temp_ids;

        // mark
        for (auto& [break_id, fail_code]  : engine_data.failed_breaks)
        {
            if (fail_code == FailedBreakCode::SameClient)
            {
                auto it = std::find_if(engine_data.prev_bookings.begin(),
                              engine_data.prev_bookings.end(),
                              [&break_id = break_id](BookingRecord book_rec){ return break_id == book_rec.schedule_id; });

                if (it != engine_data.prev_bookings.end())
                {
                    if ((*it).booked_spot.brand_id != engine_data.spot_to_book.brand_id){
                        ++m_failed_break_count;
                        qDebug() << "Break ID: "<< break_id;
                        temp_ids.push_back(break_id);
                    }
                }

            }
        }


        qDebug() << "Failed Breaks: "<< m_failed_break_count;

        // sweep
        for (auto& id : temp_ids){
            auto it = std::find_if(engine_data.failed_breaks.begin(),
                          engine_data.failed_breaks.end(),
                          [&id](auto failed_break){ return std::get<0>(failed_break) == id; });

            if (it != engine_data.failed_breaks.end())
                engine_data.failed_breaks.erase(it);


            auto iter = std::find_if(engine_data.m_schedule_EDM->modelEntities().begin(),
                        engine_data.m_schedule_EDM->modelEntities().end(),
                        [&id](auto& model_entity){
                            auto target_break = dynamic_cast<Break*>(std::get<1>(model_entity).get());
                            return (target_break->id() == id); } );

            if (iter != engine_data.m_schedule_EDM->modelEntities().end()){
                            //auto& [name, entity] = *iter;
                            auto target_break = dynamic_cast<Break*>(std::get<1>(*iter).get());
                            target_break->set_break_availability(Schedule::BreakAvailability::Break_Available);
                            //target_break->setIsBad(false);
            }

        }

        return true;
    }

    bool OverrideSameClientRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode OverrideSameClientRule::fail_code()
    {
        return FailedBreakCode::TypeExcl;
    }

    void OverrideSameClientRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;
    }

    int OverrideSameClientRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    /* Block Different Spots Same Audio */

    bool BlockDiffSpotSameAudioRule::m_isEnabled = true;
    int BlockDiffSpotSameAudioRule::m_failed_break_count = 0;

    BlockDiffSpotSameAudioRule::BlockDiffSpotSameAudioRule()
    {
    }

    BlockDiffSpotSameAudioRule::~BlockDiffSpotSameAudioRule()
    {
    }

    bool BlockDiffSpotSameAudioRule::execute(EngineData& engine_data,
                                    [[maybe_unused]] const Break& target_break)
    {
        for (auto& booked_record: engine_data.prev_bookings) {

            if (booked_record.schedule_id != target_break.id())
                continue;

            qDebug() << "Booked Spot Audio ID: "<< booked_record.booked_spot.audio_id  << "Spot to Book Audio ID: "<< engine_data.spot_to_book.audio_id;

            if (booked_record.booked_spot.audio_id == engine_data.spot_to_book.audio_id) {
                ++m_failed_break_count;
                qDebug() << " === BlockDiffSpotSameAudio === ";
                qDebug() << "Booked Spot: " << booked_record.booked_spot.spot_id;
                qDebug() << "Booked Audio: "<< booked_record.booked_spot.audio_id;
                return false;
            }
        }

        return true;
    }

    bool BlockDiffSpotSameAudioRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode BlockDiffSpotSameAudioRule::fail_code()
    {
        return FailedBreakCode::DiffSpotSameAudio;
    }

    void BlockDiffSpotSameAudioRule::enable_or_disable(bool state)
    {
        m_isEnabled = state;
    }

    int BlockDiffSpotSameAudioRule::failed_break_count()
    {
        return m_failed_break_count;
    }


}
