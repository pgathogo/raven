#include "schedule.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"

#include "../audio/audio.h"


Schedule::Schedule()
    :mIsBad{false},
     m_break_availability{Schedule::Break_Available}
{
    // create fields
    m_schedule_date = createField<DateField>("schedule_date", "Schedule Date:");
    m_schedule_time = createField<TimeField>("schedule_time", "Schedule Time:");
    m_schedule_hour = createField<IntegerField>("schedule_hour", "Schedule Hour");

    m_audio = createField<ForeignKeyField>("audio_id", "Audio",
                                      std::make_unique<AUDIO::Audio>(""), "title");

    m_fade_in = createField<IntegerField>("fade_in", "Fade In");
    m_fade_out = createField<IntegerField>("fade_out", "Fade Out");
    m_fade_delay = createField<IntegerField>("fade_delay", "Fade Delay");

    m_play_status = createField<ChoiceField<std::string>>("play_status", "Play Status");
    m_play_status->addChoice({"CUED", "CUED"});
    m_play_status->addChoice({"PLAYED", "PLAYED"});
    m_play_status->addChoice({"CANCELLED", "CANCELLED"});

    m_play_date = createField<DateField>("play_date", "Play Date:");
    m_play_time = createField<TimeField>("play_time", "Play Time:");

    m_auto_transition = createField<IntegerField>("auto_transition", "Auto Transition");
    m_live_transition = createField<IntegerField>("live_transition", "Live Transition");

    m_audio_type = createField<ChoiceField<std::string>>("audio_type", "Audio Type");
    m_audio_type->addChoice({"SONG", "Song"});
    m_audio_type->addChoice({"COMM", "Commercial"});

    m_break_duration = createField<IntegerField>("break_duration", "Break Duration");
    m_break_start_win = createField<IntegerField>("break_start_win", "Break Start Win");
    m_break_end_win = createField<IntegerField>("break_end_win", "Break End Win");
    m_break_max_spots = createField<IntegerField>("break_max_spots", "Break Max Spots");
    m_booked_spots = createField<IntegerField>("booked_spots", "Booked Spots");
    m_break_duration_left = createField<IntegerField>("break_duration_left", "Break Time Left");

    m_break_mode = createField<ChoiceField<std::string>>("break_mode", "Break Mode");
    m_break_mode->addChoice({"SINGLE", "Single"});
    m_break_mode->addChoice({"MIXED", "Mixed"});

    m_break_status = createField<ChoiceField<std::string>>("break_status", "Break Status");
    m_break_status->addChoice({"LOCKED", "Locked"});
    m_break_status->addChoice({"OPEN", "Open"});

    m_comment = createField<TextField>("comment", "Comment");

    m_header << stoq(m_schedule_time->fieldLabel())
            << stoq(m_break_mode->fieldLabel())
            << stoq(m_break_max_spots->fieldLabel())
            << stoq(m_break_duration->fieldLabel())
            << stoq(m_booked_spots->fieldLabel())
            << stoq(m_break_duration_left->fieldLabel())
            << stoq(m_break_status->fieldLabel());

    setTableName("rave_schedule");

    setDefaults();
}

Schedule::~Schedule()
{
}

DateField *Schedule::scheduleDate() const
{
    return m_schedule_date;
}

DateField* Schedule::setScheduleDate(const QDate val)
{
    m_schedule_date->setValue( val );
    return m_schedule_date;
}

TimeField *Schedule::scheduleTime() const
{
    return m_schedule_time;
}

TimeField* Schedule::setScheduleTime(const QTime val)
{
    m_schedule_time->setValue( val );
    return m_schedule_time;
}

IntegerField *Schedule::scheduleHour() const
{
    return m_schedule_hour;
}

IntegerField* Schedule::setScheduleHour(int val)
{
    m_schedule_hour->setValue( val );
    return m_schedule_hour;
}

ForeignKeyField* Schedule::audio() const
{
    return m_audio;
}

void Schedule::set_audio(int val)
{
    m_audio->setValue( val );
}

IntegerField *Schedule::fadeIn() const
{
    return m_fade_in;
}

void Schedule::setFadeIn(int val)
{
    m_fade_in->setValue( val );
}

IntegerField *Schedule::fadeOut() const
{
    return m_fade_out;
}

void Schedule::setFadeOut(int val)
{
    m_fade_out->setValue( val );
}

IntegerField *Schedule::fadeDelay() const
{
    return m_fade_delay;
}

void Schedule::setFadeDelay(int val)
{
    m_fade_delay->setValue( val );
}

ChoiceField<std::string> *Schedule::playStatus() const
{
    return m_play_status;
}

void Schedule::setPlayStatus(const std::string val)
{
    m_play_status->setValue( val );
}

DateField *Schedule::playDate() const
{
    return m_play_date;
}

void Schedule::setPlayDate(QDate val)
{
    m_play_date->setValue( val );
}

TimeField *Schedule::playTime() const
{
    return m_play_time;
}

void Schedule::setPlayTime(QTime val)
{
    m_play_time->setValue(val);
}

IntegerField *Schedule::autoTransition() const
{
    return m_auto_transition;
}

void Schedule::setAutoTransition(int val)
{
    m_auto_transition->setValue( val );
}

IntegerField *Schedule::liveTransition() const
{
    return m_live_transition;
}

void Schedule::setLiveTransition(int val)
{
    m_live_transition->setValue( val );
}

ChoiceField<std::string> *Schedule::audio_type() const
{
    return m_audio_type;
}

ChoiceField<std::string>* Schedule::set_audio_type(std::string val)
{
    m_audio_type->setValue(val);
    return m_audio_type;
}

IntegerField *Schedule::breakDuration() const
{
    return m_break_duration;
}

IntegerField* Schedule::setBreakDuration(int val)
{
    m_break_duration->setValue(val);
    return m_break_duration;
}

IntegerField *Schedule::breakStartWin() const
{
    return m_break_start_win;
}

void Schedule::setBreakStartWin(int val)
{
    m_break_start_win->setValue( val );
}

IntegerField *Schedule::breakEndWin() const
{
    return m_break_end_win;
}

void Schedule::setBreakEndWin(int val)
{
    m_break_end_win->setValue(val);
}

IntegerField *Schedule::breakMaxSpots() const
{
    return m_break_max_spots;
}

IntegerField* Schedule::setBreakMaxSpots(int val)
{
    m_break_max_spots->setValue(val);
    return m_break_max_spots;
}

IntegerField* Schedule::bookedSpots() const
{
    return m_booked_spots;
}

IntegerField* Schedule::setBookedSpots(int val)
{
    m_booked_spots->setValue(val);
    return m_booked_spots;
}

IntegerField *Schedule::breakDurationLeft() const
{
    return m_break_duration_left;
}

IntegerField* Schedule::setBreakDurationLeft(int val)
{
    m_break_duration_left->setValue( val );
    return m_break_duration_left;
}

ChoiceField<std::string> *Schedule::breakMode() const
{
    return m_break_mode;
}

ChoiceField<std::string>* Schedule::setBreakMode(const std::string val)
{
    m_break_mode->setValue(val);
    return m_break_mode;
}

ChoiceField<std::string> *Schedule::breakStatus() const
{
    return m_break_status;
}

void Schedule::setBreakStatus(const std::string val)
{
    m_break_status->setValue(val);
}

TextField *Schedule::comment() const
{
    return m_comment;
}

void Schedule::setComment(const std::string val)
{
    m_comment->setValue(val);
}

bool Schedule::isBad() const
{
    return mIsBad;
}

void Schedule::setIsBad(bool flag)
{
    mIsBad = flag;
}

void Schedule::set_break_availability(BreakAvailability break_availability)
{
    m_break_availability = break_availability;

}

Schedule::BreakAvailability Schedule::break_availability() const
{
    if (m_break_availability == BreakAvailability::Break_Available)
        return BreakAvailability::Break_Available;
    else
        return BreakAvailability::Break_Not_Available;
}

std::string Schedule::tableName() const
{
    return m_table_name;
}

void Schedule::setTableName(const std::string table_name)
{
    m_table_name = table_name;
}

std::unique_ptr<BaseEntity> Schedule::mapFields(StringMap*)
{
}

std::vector<std::string> Schedule::tableViewColumns() const
{
    return m_view_columns;
}

std::vector<std::string> Schedule::tableViewValues()
{
    return m_view_values;
}

QStringList Schedule::tableHeaders() const
{
    return m_header;
}

std::string Schedule::searchColumn() const
{
    return "";
}

void Schedule::populateEntity()
{
}

std::unique_ptr<BaseEntity> Schedule::cloneAsUnique()
{
    return std::make_unique<Schedule>();
}

void Schedule::afterMapping(BaseEntity&)
{

}

void Schedule::setDefaults()
{
    std::vector<std::string> cols;
    cols.push_back(scheduleTime()->displayName());
    cols.push_back(breakMaxSpots()->displayName());
    cols.push_back(breakDurationLeft()->displayName());
    cols.push_back(bookedSpots()->displayName());
    setTableViewColumns(cols);

    setTableViewValues({scheduleTime()->displayName(),
                breakMaxSpots()->displayName(),
                breakDurationLeft()->displayName(),
                bookedSpots()->displayName()});
}

void Schedule::setTableViewColumns(const std::vector<std::string> cols)
{
    m_view_columns = cols;
}

void Schedule::setTableViewValues(const std::vector<std::string> vals)
{
    m_view_values = vals;
}

std::string Schedule::make_insert_stmt(const std::vector<Field*>& fields)
{
    std::string columns = make_columns(fields);
    std::string values = make_values(fields);
    std::string insert_stmt  = "INSERT INTO "+tableName()+"("+columns+") VALUES ( "+values+");";
    return insert_stmt;
}

std::string Schedule::make_columns(const std::vector<Field*>& fields)
{
    std::string cols{};
    std::size_t i = 1;
    for(auto& fld : fields){
        cols += fld->dbColumnName();
        if (i < fields.size())
            cols +=",";
        ++i;
    }
    return cols;
}

std::string Schedule::make_values(const std::vector<Field*>& fields)
{
    std::string vals{};

    for(auto& field : fields){
        if (!vals.empty())
            vals +=",";
        vals += field->dbValueFormatter();
    }
    return vals;
}




