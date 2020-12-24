#ifndef ORDERBOOKING_H
#define ORDERBOOKING_H

#include "../framework/baseentity.h"

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

    DateField* play_date() const;
    void set_play_date(QDate val);

    DateTimeField* play_time() const;
    void set_play_time(QDateTime val);

    ForeignKeyField* spot() const;
    void set_spot(int val);

    ForeignKeyField* audio() const;
    void set_audio(int val);

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
private:
    ForeignKeyField* m_booking_segment;
    ForeignKeyField* m_schedule;
    ChoiceField<std::string>* m_booking_status;
    DateField* m_play_date;
    DateTimeField* m_play_time;
    ForeignKeyField* m_spot;
    ForeignKeyField* m_audio;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ORDERBOOKING_H
