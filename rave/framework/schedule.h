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

    IntegerField* display_row_id() const;
    void set_display_row_id(int);

    DateField* schedule_date() const;
    DateField* set_schedule_date(const QDate);

    TimeField* schedule_time() const;
    TimeField* set_schedule_time(const QTime);

    IntegerField* schedule_hour() const;
    IntegerField* set_schedule_hour(int);

    ForeignKeyField* audio() const;
    void set_audio(int val);

    IntegerField* fade_in() const;
    void set_fade_in(int val);

    IntegerField* fade_out() const;
    void set_fade_out(int val);

    IntegerField* fade_delay() const;
    void set_fade_delay(int val);

    ChoiceField<std::string>* play_status() const;
    void set_play_status(const std::string val);

    DateField* play_date() const;
    void set_play_date(QDate val);

    TimeField* play_time() const;
    void set_play_time(QTime val);

    IntegerField* auto_transition() const;
    void set_auto_transition(int val);

    IntegerField* live_transition() const;
    void set_live_transition(int val);

    ChoiceField<std::string>* schedule_item_type() const;
    ChoiceField<std::string>* set_schedule_item_type(std::string val);

    IntegerField* break_duration() const;
    IntegerField* set_break_duration(int val);

    IntegerField* break_start_win() const;
    void set_break_start_win(int val);

    IntegerField* break_end_win() const;
    void set_break_win(int val);

    IntegerField* break_max_spots() const;
    IntegerField* set_break_max_spots(int val);

    IntegerField* booked_spots() const;
    IntegerField* set_booked_spots(int val);

    IntegerField* break_duration_left() const;
    IntegerField* set_break_duration_left(int val);

    ChoiceField<std::string>* break_mode() const;
    ChoiceField<std::string>* set_break_mode(const std::string val);

    ChoiceField<std::string>* break_status() const;
    void set_break_status(const std::string val);

    StringField* break_fill_method() const;
    StringField* set_break_fill_method(std::string);

    TextField* comment()const;
    void set_comment(const std::string val);

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

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

    void setDefaults();
    void setTableViewColumns(const std::vector<std::string> cols);
    void setTableViewValues(const std::vector<std::string> vals);

    std::string make_insert_stmt(const std::vector<Field*>& fields);
    std::string make_columns(const std::vector<Field*>& fields);
    std::string make_values(const std::vector<Field*>& fields);

    void populate_audio_fk(int audio_id);

private:
    IntegerField* m_display_row_id;
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
    StringField* m_break_fill_method;
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
