#include "schedule.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"

Schedule::Schedule()
{
    // create fields
    mScheduleDate = createField<DateField>("schedule_date", "Schedule Date:");
    mScheduleTime = createField<TimeField>("schedule_time", "Schedule Time:");
    mScheduleHour = createField<IntegerField>("schedule_hour", "Schedule Hour");
    mTrack = createField<IntegerField>("track_id", "Track Reference");
    mFadeIn = createField<IntegerField>("fade_in", "Fade In");
    mFadeOut = createField<IntegerField>("fade_out", "Fade Out");
    mFadeDelay = createField<IntegerField>("fade_delay", "Fade Delay");

    mPlayStatus = createField<ChoiceField<std::string>>("play_status", "Play Status");
    mPlayStatus->addChoice({"CUED", "CUED"});
    mPlayStatus->addChoice({"PLAYED", "PLAYED"});
    mPlayStatus->addChoice({"CANCELLED", "CANCELLED"});

    mPlayDate = createField<DateField>("play_date", "Play Date:");
    mPlayTime = createField<TimeField>("play_time", "Play Time:");

    mAutoTransition = createField<IntegerField>("auto_transition", "Auto Transition");
    mLiveTransition = createField<IntegerField>("live_transition", "Live Transition");

    mTrackType = createField<ChoiceField<std::string>>("track_type", "Track Type");
    mTrackType->addChoice({"SONG", "Song"});
    mTrackType->addChoice({"COMM", "Commercial"});

    mBreakDuration = createField<IntegerField>("break_duration", "Break Duration");
    mBreakStartWin = createField<IntegerField>("break_start_win", "Break Start Win");
    mBreakEndWin = createField<IntegerField>("break_end_win", "Break End Win");
    mBreakMaxSpots = createField<IntegerField>("break_max_spots", "Break Max Spots");
    mBookedSpots = createField<IntegerField>("booked_spots", "Booked Spots");
    mBreakDurationLeft = createField<IntegerField>("break_duration_left", "Break Time Left");

    mBreakMode = createField<ChoiceField<std::string>>("break_mode", "Break Mode");
    mBreakMode->addChoice({"SINGLE", "Single"});
    mBreakMode->addChoice({"MIXED", "Mixed"});

    mBreakStatus = createField<ChoiceField<std::string>>("break_status", "Break Status");
    mBreakStatus->addChoice({"LOCKED", "Locked"});
    mBreakStatus->addChoice({"OPEN", "Open"});

    mComment = createField<TextField>("comment", "Comment");

    mHeader << stoq(mScheduleTime->fieldLabel())
            << stoq(mBreakMode->fieldLabel())
            << stoq(mBreakMaxSpots->fieldLabel())
            << stoq(mBreakDuration->fieldLabel())
            << stoq(mBookedSpots->fieldLabel())
            << stoq(mBreakDurationLeft->fieldLabel())
            << stoq(mBreakStatus->fieldLabel());

    setTableName("rave_schedule");

    setDefaults();
}

Schedule::~Schedule()
{
}

DateField *Schedule::scheduleDate() const
{
    return mScheduleDate;
}

DateField* Schedule::setScheduleDate(const QDate val)
{
    mScheduleDate->setValue( val );
    return mScheduleDate;
}

TimeField *Schedule::scheduleTime() const
{
    return mScheduleTime;
}

TimeField* Schedule::setScheduleTime(const QTime val)
{
    mScheduleTime->setValue( val );
    return mScheduleTime;
}

IntegerField *Schedule::scheduleHour() const
{
    return mScheduleHour;
}

IntegerField* Schedule::setScheduleHour(int val)
{
    mScheduleHour->setValue( val );
    return mScheduleHour;
}

IntegerField* Schedule::track() const
{
    return mTrack;
}

void Schedule::setTrack(int val)
{
    mTrack->setValue( val );
}

IntegerField *Schedule::fadeIn() const
{
    return mFadeIn;
}

void Schedule::setFadeIn(int val)
{
    mFadeIn->setValue( val );
}

IntegerField *Schedule::fadeOut() const
{
    return mFadeOut;
}

void Schedule::setFadeOut(int val)
{
    mFadeOut->setValue( val );
}

IntegerField *Schedule::fadeDelay() const
{
    return mFadeDelay;
}

void Schedule::setFadeDelay(int val)
{
    mFadeDelay->setValue( val );
}

ChoiceField<std::string> *Schedule::playStatus() const
{
    return mPlayStatus;
}

void Schedule::setPlayStatus(const std::string val)
{
    mPlayStatus->setValue( val );
}

DateField *Schedule::playDate() const
{
    return mPlayDate;
}

void Schedule::setPlayDate(QDate val)
{
    mPlayDate->setValue( val );
}

TimeField *Schedule::playTime() const
{
    return mPlayTime;
}

void Schedule::setPlayTime(QTime val)
{
    mPlayTime->setValue(val);
}

IntegerField *Schedule::autoTransition() const
{
    return mAutoTransition;
}

void Schedule::setAutoTransition(int val)
{
    mAutoTransition->setValue( val );
}

IntegerField *Schedule::liveTransition() const
{
    return mLiveTransition;
}

void Schedule::setLiveTransition(int val)
{
    mLiveTransition->setValue( val );
}

ChoiceField<std::string> *Schedule::trackType() const
{
    return mTrackType;
}

ChoiceField<std::string>* Schedule::setTrackType(std::string val)
{
    mTrackType->setValue(val);
    return mTrackType;

}

IntegerField *Schedule::breakDuration() const
{
    return mBreakDuration;
}

IntegerField* Schedule::setBreakDuration(int val)
{
    mBreakDuration->setValue(val);
    return mBreakDuration;
}

IntegerField *Schedule::breakStartWin() const
{
    return mBreakStartWin;
}

void Schedule::setBreakStartWin(int val)
{
    mBreakStartWin->setValue( val );
}

IntegerField *Schedule::breakEndWin() const
{
    return mBreakEndWin;
}

void Schedule::setBreakEndWin(int val)
{
    mBreakEndWin->setValue(val);
}

IntegerField *Schedule::breakMaxSpots() const
{
    return mBreakMaxSpots;
}

IntegerField* Schedule::setBreakMaxSpots(int val)
{
    mBreakMaxSpots->setValue(val);
    return mBreakMaxSpots;
}

IntegerField* Schedule::bookedSpots() const
{
    return mBookedSpots;
}

IntegerField* Schedule::setBookedSpots(int val)
{
    mBookedSpots->setValue(val);
    return mBookedSpots;
}

IntegerField *Schedule::breakDurationLeft() const
{
    return mBreakDurationLeft;
}

IntegerField* Schedule::setBreakDurationLeft(int val)
{
    mBreakDurationLeft->setValue( val );
    return mBreakDurationLeft;
}

ChoiceField<std::string> *Schedule::breakMode() const
{
    return mBreakMode;
}

ChoiceField<std::string>* Schedule::setBreakMode(const std::string val)
{
    mBreakMode->setValue(val);
    return mBreakMode;
}

ChoiceField<std::string> *Schedule::breakStatus() const
{
    return mBreakStatus;
}

void Schedule::setBreakStatus(const std::string val)
{
    mBreakStatus->setValue(val);
}

TextField *Schedule::comment() const
{
    return mComment;
}

void Schedule::setComment(const std::string val)
{
    mComment->setValue(val);
}

std::string Schedule::tableName() const
{
    return mTableName;
}

void Schedule::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> Schedule::mapFields(StringMap*)
{
}

std::vector<std::string> Schedule::tableViewColumns() const
{
    return viewColumns;
}

std::vector<std::string> Schedule::tableViewValues()
{
    return viewValues;
}

QStringList Schedule::tableHeaders() const
{
    return mHeader;
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
    viewColumns = cols;
}

void Schedule::setTableViewValues(const std::vector<std::string> vals)
{
    viewValues = vals;
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




