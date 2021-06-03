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
    enum BreakAvailability{Break_Not_Available, Break_Available};

    Schedule();
    ~Schedule() override;

    DateField* scheduleDate() const;
    DateField* setScheduleDate(const QDate val);

    TimeField* scheduleTime() const;
    TimeField* setScheduleTime(const QTime val);

    IntegerField* scheduleHour() const;
    IntegerField* setScheduleHour(int val);

    ForeignKeyField* audio() const;
    void set_audio(int val);

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

    ChoiceField<std::string>* audio_type() const;
    ChoiceField<std::string>* set_audio_type(std::string val);

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

    bool isBad() const;
    void setIsBad(bool flag);

    void set_break_availability(BreakAvailability break_availability);
    BreakAvailability break_availability() const;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;

    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    void setDefaults();
    void setTableViewColumns(const std::vector<std::string> cols);
    void setTableViewValues(const std::vector<std::string> vals);

    std::string make_insert_stmt(const std::vector<Field*>& fields);
    std::string make_columns(const std::vector<Field*>& fields);
    std::string make_values(const std::vector<Field*>& fields);

private:
    DateField* m_schedule_date;
    TimeField* m_schedule_time;
    IntegerField* m_schedule_hour;
    ForeignKeyField* m_audio;
    IntegerField* m_fade_in;
    IntegerField* m_fade_out;
    IntegerField* m_fade_delay;
    ChoiceField<std::string>* m_play_status;
    DateField* m_play_date;
    TimeField* m_play_time;
    IntegerField* m_auto_transition;
    IntegerField* m_live_transition;
    ChoiceField<std::string>* m_schedule_item_type;
    IntegerField* m_break_duration;
    IntegerField* m_break_start_win;
    IntegerField* m_break_end_win;
    IntegerField* m_break_max_spots;
    IntegerField* m_booked_spots;
    IntegerField* m_break_duration_left;
    ChoiceField<std::string>* m_break_mode;
    ChoiceField<std::string>* m_break_status;
    TextField* m_comment;

    int m_break_availability;

    bool mIsBad;

    QStringList m_header;
    std::string m_table_name;
    std::vector<std::string> m_view_columns;
    std::vector<std::string> m_view_values;
};

#endif // SCHEDULE_H
