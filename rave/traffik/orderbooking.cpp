#include "../framework/choicefield.h"
#include "orderbooking.h"
#include "bookingsegment.h"
#include "schedule.h"
#include "spotaudio.h"

OrderBooking::OrderBooking()
{
    mSchedule = createField<ForeignKeyField>("schedule_id", "Schedule",
                                             std::make_unique<Schedule>(), "schedule_date");

    mBookingSegment = createField<ForeignKeyField>("bookingsegment_id", "Booking Segment",
                                                   std::make_unique<BookingSegment>(), "booking_date");

    mBookingStatus = createField<ChoiceField<std::string>>("booking_status", "Booking Status");
    mBookingStatus->addChoice({"READY","READY"});
    mBookingStatus->addChoice({"PLAYED","PLAYED"});
    mBookingStatus->addChoice({"CANCEL","CANCEL"});

    mPlayDate = createField<DateField>("play_date", "Play Date");

    mPlayTime = createField<TimeField>("play_time", "Play Time");

    mAudio = createField<ForeignKeyField>("audio_id", "Audio",
                                          std::make_unique<SpotAudio>(), "spot_id");

    mHeader << QString::fromStdString(schedule()->displayName());

    setTableName("rave_orderbooking");

}

OrderBooking::~OrderBooking()
{

}

ForeignKeyField *OrderBooking::bookingSegment() const
{
    return mBookingSegment;
}

void OrderBooking::setBookingSegment(int val)
{
    mBookingSegment->setValue(val);
}

ForeignKeyField *OrderBooking::schedule() const
{
    return mSchedule;
}

void OrderBooking::setSchedule(int val)
{
    mSchedule->setValue(val);
}

ChoiceField<std::string>* OrderBooking::bookingStatus() const
{
    return mBookingStatus;
}

void OrderBooking::setBookingStatus(std::string val)
{
    mBookingStatus->setValue(val);
}

DateField *OrderBooking::playDate() const
{
    return mPlayDate;
}

void OrderBooking::setPlayDate(QDate val)
{
    mPlayDate->setValue(val);
}

TimeField *OrderBooking::playTime() const
{
    return mPlayTime;
}

void OrderBooking::setPlayTime(QTime val)
{
    mPlayTime->setValue(val);
}

ForeignKeyField *OrderBooking::spot() const
{
    return mSpot;
}

void OrderBooking::setSpot(int val)
{
    mSpot->setValue(val);
}

ForeignKeyField *OrderBooking::audio() const
{
    return mAudio;
}

void OrderBooking::setAudio(int val)
{
    mAudio->setValue(val);
}

std::string OrderBooking::tableName() const
{
    return mTableName;
}

void OrderBooking::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> OrderBooking::mapFields(StringMap *raw_entity)
{
}

std::vector<std::string> OrderBooking::tableViewColumns() const
{
    return tableViewCols<std::string>(
                schedule()->displayName()
                );
}

std::vector<std::string> OrderBooking::tableViewValues()
{
    return tableViewColumns();
}

QStringList OrderBooking::tableHeaders() const
{
    return mHeader;
}

std::string OrderBooking::searchColumn() const
{
    return schedule()->valueToString();
}

void OrderBooking::populateEntity()
{
}

std::unique_ptr<BaseEntity> OrderBooking::cloneAsUnique()
{
    return std::make_unique<OrderBooking>();
}

void OrderBooking::afterMapping(BaseEntity &entity)
{
}
