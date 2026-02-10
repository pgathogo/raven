#ifndef ORDERBOOKING_H
#define ORDERBOOKING_H

#include "../../../rave/framework/baseentity.h"

template<typename T>
class ChoiceField;

class OrderBooking : public BaseEntity
{
public:
    OrderBooking();
    ~OrderBooking() override;

    ForeignKeyField* booking_segment() const;
    void set_booking_segment(int val);

    ForeignKeyField* schedule() const;
    void set_schedule(int val);

    ChoiceField<std::string>* booking_status() const;
    void set_booking_status(std::string val);

    DateField* book_date();
    void set_book_date(QDate);

    TimeField* book_time();
    void set_book_time(QTime);

    IntegerField* book_hour();
    void set_book_hour(int);

    IntegerField* book_seq();
    void set_book_seq(int);

    DateField* play_date() const;
    void set_play_date(QDate val);

    DateTimeField* play_time() const;
    void set_play_time(QDateTime val);

    ForeignKeyField* spot() const;
    void set_spot(int val);

    ForeignKeyField* audio() const;
    void set_audio(int val);

    IntegerField* played_audio();
    void set_played_audio(int);

    ForeignKeyField* order() const;
    void set_order(int);

    ForeignKeyField* void_reason();
    void set_void_reason(int);

    TextField* comment();
    void set_comment(const std::string);

    StringField* add_login();
    void set_add_login(const std::string);

    DateTimeField* add_dtime();
    void set_add_dtime(QDateTime);

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
private:
    ForeignKeyField* m_booking_segment;
    ForeignKeyField* m_schedule;
    ChoiceField<std::string>* m_booking_status;
    DateField* m_play_date;
    DateTimeField* m_play_time;
    ForeignKeyField* m_spot;
    ForeignKeyField* m_spot_audio;
    IntegerField* m_played_audio;
    DateField* m_book_date;
    TimeField* m_book_time;
    IntegerField* m_book_hour;
    IntegerField* m_book_seq;
    ForeignKeyField* m_order;

    ForeignKeyField* m_void_reason;
    TextField* m_comment;
    StringField* m_add_login;
    DateTimeField* m_add_dtime;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ORDERBOOKING_H
