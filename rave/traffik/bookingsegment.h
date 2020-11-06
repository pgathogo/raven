#ifndef BOOKINGSEGMENT_H
#define BOOKINGSEGMENT_H

#include "../framework/baseentity.h"

class BookingSegment : public BaseEntity
{
public:
    BookingSegment();
    ~BookingSegment() override;

    ForeignKeyField* order() const;
    void setOrder(int val);

    DateField* bookingDate() const;
    void setBookingDate(QDate val);

    DateField* startDate() const;
    void setStartDate(QDate val);

    DateField* endDate() const;
    void setEndDate(QDate val);

    IntegerField* bookingCount() const;
    void setBookingCount(int val);

    DecimalField* segmentValue() const;
    void setSegmentValue(double val);

    TextField* comments() const;
    void setComments(const std::string val);

    StringField* addLogin() const;
    void setAddLogin(const std::string val);

    DateTimeField* addDateTime() const;
    void setAddDateTime(QDateTime val);

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
    ForeignKeyField* mOrder;
    DateField* mBookingDate;
    DateField* mStartDate;
    DateField* mEndDate;
    IntegerField* mBookingCount;
    DecimalField* mSegmentValue;
    TextField* mComments;
    StringField* mAddLogin;
    DateTimeField* mAddDateTime;

    QStringList mHeader;
    std::string mTableName;
};

#endif // BOOKINGSEGMENT_H
