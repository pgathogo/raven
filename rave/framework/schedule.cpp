#include "schedule.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"

#include "../audio/audio.h"


Schedule::Schedule()
    :mIsBad{false}
     ,m_break_availability{Schedule::Break_Available}
{
    // create fields
    m_display_row_id = createField<IntegerField>("display_row_id", "Display Row Id");
    m_display_row_id->setFormOnly(true);
    m_display_row_id->setValue(-1);

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

    m_schedule_item_type = createField<ChoiceField<std::string>>("schedule_item_type", "Item Type");
    m_schedule_item_type->addChoice({"SONG", "Song"});
    m_schedule_item_type->addChoice({"COMM-BREAK", "Commercial"});

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

IntegerField* Schedule::display_row_id() const
{
    return m_display_row_id;
}

void Schedule::set_display_row_id(int id)
{
    m_display_row_id->setValue(id);
}

DateField *Schedule::schedule_date() const
{
    return m_schedule_date;
}

DateField* Schedule::set_schedule_date(const QDate val)
{
    m_schedule_date->setValue( val );
    return m_schedule_date;
}

TimeField *Schedule::schedule_time() const
{
    return m_schedule_time;
}

TimeField* Schedule::set_schedule_time(const QTime val)
{
    m_schedule_time->setValue( val );
    return m_schedule_time;
}

IntegerField *Schedule::schedule_hour() const
{
    return m_schedule_hour;
}

IntegerField* Schedule::set_schedule_hour(int val)
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

IntegerField *Schedule::fade_in() const
{
    return m_fade_in;
}

void Schedule::set_fade_in(int val)
{
    m_fade_in->setValue( val );
}

IntegerField *Schedule::fade_out() const
{
    return m_fade_out;
}

void Schedule::set_fade_out(int val)
{
    m_fade_out->setValue( val );
}

IntegerField *Schedule::fade_delay() const
{
    return m_fade_delay;
}

void Schedule::set_fade_delay(int val)
{
    m_fade_delay->setValue( val );
}

ChoiceField<std::string> *Schedule::play_status() const
{
    return m_play_status;
}

void Schedule::set_play_status(const std::string val)
{
    m_play_status->setValue( val );
}

DateField *Schedule::play_date() const
{
    return m_play_date;
}

void Schedule::set_play_date(QDate val)
{
    m_play_date->setValue( val );
}

TimeField *Schedule::play_time() const
{
    return m_play_time;
}

void Schedule::set_play_time(QTime val)
{
    m_play_time->setValue(val);
}

IntegerField *Schedule::auto_transition() const
{
    return m_auto_transition;
}

void Schedule::set_auto_transition(int val)
{
    m_auto_transition->setValue( val );


}

IntegerField *Schedule::live_transition() const
{
    return m_live_transition;
}

void Schedule::set_live_transition(int val)
{
    m_live_transition->setValue( val );
}

ChoiceField<std::string> *Schedule::schedule_item_type() const
{
    return m_schedule_item_type;
}

ChoiceField<std::string>* Schedule::set_schedule_item_type(std::string val)
{
    m_schedule_item_type->setValue(val);
    m_schedule_item_type->setCurrText(val);
    return m_schedule_item_type;
}

IntegerField *Schedule::break_duration() const
{
    return m_break_duration;
}

IntegerField* Schedule::set_break_duration(int val)
{
    m_break_duration->setValue(val);
    return m_break_duration;
}

IntegerField *Schedule::break_start_win() const
{
    return m_break_start_win;
}

void Schedule::set_break_start_win(int val)
{
    m_break_start_win->setValue( val );
}

IntegerField *Schedule::break_end_win() const
{
    return m_break_end_win;
}

void Schedule::set_break_win(int val)
{
    m_break_end_win->setValue(val);
}

IntegerField *Schedule::break_max_spots() const
{
    return m_break_max_spots;
}

IntegerField* Schedule::set_break_max_spots(int val)
{
    m_break_max_spots->setValue(val);
    return m_break_max_spots;
}

IntegerField* Schedule::booked_spots() const
{
    return m_booked_spots;
}

IntegerField* Schedule::set_booked_spots(int val)
{
    m_booked_spots->setValue(val);
    return m_booked_spots;
}

IntegerField *Schedule::break_duration_left() const
{
    return m_break_duration_left;
}

IntegerField* Schedule::set_break_duration_left(int val)
{
    m_break_duration_left->setValue( val );
    return m_break_duration_left;
}

ChoiceField<std::string> *Schedule::break_mode() const
{
    return m_break_mode;
}

ChoiceField<std::string>* Schedule::set_break_mode(const std::string val)
{
    m_break_mode->setValue(val);
    return m_break_mode;
}

ChoiceField<std::string> *Schedule::break_status() const
{
    return m_break_status;
}

void Schedule::set_break_status(const std::string val)
{
    m_break_status->setValue(val);
}

TextField *Schedule::comment() const
{
    return m_comment;
}

void Schedule::set_comment(const std::string val)
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

void Schedule::afterMapping(BaseEntity& entity)
{


}

void Schedule::setDefaults()
{
    std::vector<std::string> cols;
    cols.push_back(schedule_time()->displayName());
    cols.push_back(break_max_spots()->displayName());
    cols.push_back(break_duration_left()->displayName());
    cols.push_back(booked_spots()->displayName());
    setTableViewColumns(cols);

    setTableViewValues({schedule_time()->displayName(),
                break_max_spots()->displayName(),
                break_duration_left()->displayName(),
                booked_spots()->displayName()});
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

void Schedule::populate_audio_fk(int audio_id)
{
    EntityDataModel edm;
    auto filter = std::make_tuple("id", " = ", audio_id);
    std::string filter_str = edm.prepareFilter(filter);

    m_audio = createField<ForeignKeyField>("audio_id", "Audio",
                                      std::make_unique<AUDIO::Audio>(""),
                                      "title",
                                       filter_str);
}




