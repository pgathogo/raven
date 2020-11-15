#include <algorithm>
#include <QCheckBox>
#include "traffikrules.h"
#include "schedule.h"

namespace TRAFFIK{

    RuleEngine::RuleEngine(EngineData &ed)
        :mEngineData{ed}
    {
        register_rule<FullBreakRule>();
        register_rule<TypeExclusionRule>();
        register_rule<VoiceExclusionRule>();
    }

    bool RuleEngine::run(Break* target_break)
    {
        /*
          - Breaks Checked
          - Full Breaks
          - Type Exclusion collusions
          - Voice Exclusion collusions
          - Type Daypart collusions
          - Voice Daypart collusions
          - Spot Daypart collusions
          - Same Client collusions
          - Override Same Client for Different brands **

          */

        bool results = true;

        for (auto& rule : m_rules ){

            if (!rule->isEnabled())
                continue;

            // If target break is logged as failed, don't pass it to other rules
            if (find_failed_break(mEngineData.failed_breaks, target_break->id())){
                results =  false;
                break;
            }

            for (auto& booking : mEngineData.prev_bookings){

                bool result = rule->execute(mEngineData,
                                            booking,
                                            *target_break);

                if (!result){
                    mEngineData.failed_breaks.push_back(std::make_tuple(target_break->id(),
                                         rule->fail_code()));
                    target_break->setIsBad(true);
                    break;
                }
            }

        }

        return results;
    }

    bool RuleEngine::find_failed_break(std::vector<FailedBreak> &fb, int break_id)
    {
        return (std::find_if(fb.begin(), fb.end(),
                      [&break_id](FailedBreak& fb){ return (std::get<0>(fb) == break_id) ? true : false; })
                != fb.end()) ? true : false;
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

    bool FullBreakRule::execute(EngineData& engine_data,
                                    [[maybe_unused]] const BookedRecord& booked_record,
                                    Break& target_break)
    {
        // verify if target break has enough space to fit the spot that's been booked
        if (target_break.breakDurationLeft()->value() <
                engine_data.spot_to_book.spot_duration){
            ++m_failed_break_count;
            return false;
        }else{
            return true;
        }
    }

    bool FullBreakRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode FullBreakRule::fail_code()
    {
        return FailedBreakCode::BreakFull;
    }

    void FullBreakRule::enable_or_disable(int state)
    {
        qDebug() << "State Changed: "<< state;

        switch(state){
         case RuleState::Enabled:
            m_isEnabled = true;
            break;
        case RuleState::Disabled:
            m_isEnabled = false;
            break;
        }

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
                                    const BookedRecord& booked_record,
                                    Break& target_break)
    {
        bool result = true;

        for (const int& k : engine_data.spot_to_book.type_ex_keys){

            if ((std::find(booked_record.booked_spot.type_ex_keys.begin(),
                                     booked_record.booked_spot.type_ex_keys.end(), k) !=
                    booked_record.booked_spot.type_ex_keys.end()) &&
              (booked_record.schedule_id == target_break.id()))
            {
                ++m_failed_break_count;
                result = false;
                break;
            }
        }

        return result;
    }

    bool TypeExclusionRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode TypeExclusionRule::fail_code()
    {
        return FailedBreakCode::TypeExcl;
    }

    void TypeExclusionRule::enable_or_disable(int state)
    {
        switch(state){
         case RuleState::Enabled:
            m_isEnabled = true;
            break;
        case RuleState::Disabled:
            m_isEnabled = false;
            break;
        }
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
                                     const BookedRecord &booked_record,
                                     Break& target_break)
    {
        bool result = true;

        for (const int& k : engine_data.spot_to_book.voice_ex_keys){

            if ((std::find(booked_record.booked_spot.voice_ex_keys.begin(),
                                     booked_record.booked_spot.voice_ex_keys.end(), k) !=
                    booked_record.booked_spot.voice_ex_keys.end()) &&
                (booked_record.schedule_id == target_break.id()))
            {
                ++m_failed_break_count;
                result = false;
                break;
            }
        }
        return result;
    }

    bool VoiceExclusionRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode VoiceExclusionRule::fail_code()
    {
        return FailedBreakCode::VoiceExcl;
    }

    void VoiceExclusionRule::enable_or_disable(int state)
    {
        switch(state){
         case RuleState::Enabled:
            m_isEnabled = true;
            break;
        case RuleState::Disabled:
            m_isEnabled = false;
            break;
        }

    }

    int VoiceExclusionRule::failed_break_count()
    {
        return m_failed_break_count;
    }

    bool TypeDaypartRule::m_isEnabled = false;
    int TypeDaypartRule::m_failed_break_count = 0;

    TypeDaypartRule::TypeDaypartRule()
    {
    }

    TypeDaypartRule::~TypeDaypartRule()
    {
    }

    bool TypeDaypartRule::execute(EngineData &engine_data,
                                  const BookedRecord &booked_record,
                                  Break &target_break)
    {

    }

    bool TypeDaypartRule::isEnabled()
    {
        return m_isEnabled;
    }

    FailedBreakCode TypeDaypartRule::fail_code()
    {

        return FailedBreakCode::TypeDaypart;
    }

    void TypeDaypartRule::enable_or_disable(int state)
    {
        switch(state){
         case RuleState::Enabled:
            m_isEnabled = true;
            break;
        case RuleState::Disabled:
            m_isEnabled = false;
            break;
        }

    }

    int TypeDaypartRule::failed_break_count()
    {
        return m_failed_break_count;
    }

}
