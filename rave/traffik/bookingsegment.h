#ifndef BOOKINGSEGMENT_H
#define BOOKINGSEGMENT_H

#include "../framework/baseentity.h"

class BookingSegment : public BaseEntity
{
public:
    BookingSegment();
    ~BookingSegment() override;

    ForeignKeyField* order() const;
    void set_order(int val);

    DateField* booking_date() const;
    void set_booking_date(QDate val);

    DateField* start_date() const;
    void set_start_date(QDate val);

    DateField* end_date() const;
    void set_end_date(QDate val);

    IntegerField* booking_count() const;
    void set_booking_count(int val);

    DecimalField* segment_value() const;
    void set_segment_value(double val);

    TextField* comments() const;
    void set_comments(const std::string val);

    StringField* add_login() const;
    void set_add_login(const std::string val);

    DateTimeField* add_date_time() const;
    void set_add_date_time(QDateTime val);

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
    ForeignKeyField* m_order;
    DateField* m_booking_date;
    DateField* m_start_date;
    DateField* m_end_date;
    IntegerField* m_booking_count;
    DecimalField* m_segment_value;
    TextField* m_comments;
    StringField* m_add_login;
    DateTimeField* m_add_date_time;

    QStringList mHeader;
    std::string mTableName;
};

#endif // BOOKINGSEGMENT_H
