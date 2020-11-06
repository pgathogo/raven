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

    ForeignKeyField* bookingSegment() const;
    void setBookingSegment(int val);

    ForeignKeyField* schedule() const;
    void setSchedule(int val);

    ChoiceField<std::string>* bookingStatus() const;
    void setBookingStatus(std::string val);

    DateField* playDate() const;
    void setPlayDate(QDate val);

    TimeField* playTime() const;
    void setPlayTime(QTime val);

    ForeignKeyField* spot() const;
    void setSpot(int val);

    ForeignKeyField* audio() const;
    void setAudio(int val);

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
    ForeignKeyField* mBookingSegment;
    ForeignKeyField* mSchedule;
    ChoiceField<std::string>* mBookingStatus;
    DateField* mPlayDate;
    TimeField* mPlayTime;
    ForeignKeyField* mSpot;
    ForeignKeyField* mAudio;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ORDERBOOKING_H
