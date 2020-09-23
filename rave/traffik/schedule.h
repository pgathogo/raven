#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../framework/baseentity.h"

class DateField;
class TimeField;

template<typename T>
class ChoiceField;


class Schedule : public BaseEntity
{
public:
    Schedule();
    ~Schedule() override;

    DateField* scheduleDate() const;
    DateField* setScheduleDate(const QDate val);

    TimeField* scheduleTime() const;
    TimeField* setScheduleTime(const QTime val);

    IntegerField* scheduleHour() const;
    IntegerField* setScheduleHour(int val);

    IntegerField* track() const;
    void setTrack(int val);

    IntegerField* fadeIn() const;
    void setFadeIn(int val);

    IntegerField* fadeOut() const;
    void setFadeOut(int val);

    IntegerField* fadeDelay() const;
    void setFadeDelay(int val);

    ChoiceField<std::string>* playStatus() const;
    void setPlayStatus(const std::string val);

    DateField* playDate() const;
    void setPlayDate(QDate val);

    TimeField* playTime() const;
    void setPlayTime(QTime val);

    IntegerField* autoTransition() const;
    void setAutoTransition(int val);

    IntegerField* liveTransition() const;
    void setLiveTransition(int val);

    ChoiceField<std::string>* trackType() const;
    ChoiceField<std::string>* setTrackType(std::string val);

    IntegerField* breakDuration() const;
    IntegerField* setBreakDuration(int val);

    IntegerField* breakStartWin() const;
    void setBreakStartWin(int val);

    IntegerField* breakEndWin() const;
    void setBreakEndWin(int val);

    IntegerField* breakMaxSpots() const;
    IntegerField* setBreakMaxSpots(int val);

    IntegerField* bookedSpots() const;
    IntegerField* setBookedSpots(int val);

    IntegerField* breakDurationLeft() const;
    IntegerField* setBreakDurationLeft(int val);

    ChoiceField<std::string>* breakMode() const;
    ChoiceField<std::string>* setBreakMode(const std::string val);

    ChoiceField<std::string>* breakStatus() const;
    void setBreakStatus(const std::string val);

    TextField* comment()const;
    void setComment(const std::string val);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;

    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    void setDefaults();
    void setTableViewColumns(const std::list<std::string> cols);
    void setTableViewValues(const std::vector<std::string> vals);

    std::string make_insert_stmt(const std::vector<Field*>& fields);
    std::string make_columns(const std::vector<Field*>& fields);
    std::string make_values(const std::vector<Field*>& fields);

private:
    DateField* mScheduleDate;
    TimeField* mScheduleTime;
    IntegerField* mScheduleHour;
    IntegerField* mTrack;   // ** to be changed to ForeignKeyField
    IntegerField* mFadeIn;
    IntegerField* mFadeOut;
    IntegerField* mFadeDelay;
    ChoiceField<std::string>* mPlayStatus;
    DateField* mPlayDate;
    TimeField* mPlayTime;
    IntegerField* mAutoTransition;
    IntegerField* mLiveTransition;
    ChoiceField<std::string>* mTrackType;
    IntegerField* mBreakDuration;
    IntegerField* mBreakStartWin;
    IntegerField* mBreakEndWin;
    IntegerField* mBreakMaxSpots;
    IntegerField* mBookedSpots;
    IntegerField* mBreakDurationLeft;
    ChoiceField<std::string>* mBreakMode;
    ChoiceField<std::string>* mBreakStatus;
    TextField* mComment;

    QStringList mHeader;
    std::string mTableName;
    std::list<std::string> viewColumns;
    std::vector<std::string> viewValues;
};

#endif // SCHEDULE_H
