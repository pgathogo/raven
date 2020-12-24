#include "../framework/choicefield.h"
#include "orderbooking.h"
#include "bookingsegment.h"
#include "schedule.h"
#include "spotaudio.h"
#include "spot.h"

OrderBooking::OrderBooking()
{
    m_schedule = createField<ForeignKeyField>("schedule_id", "Schedule",
                                             std::make_unique<Schedule>(), "schedule_date");

    m_booking_segment = createField<ForeignKeyField>("bookingsegment_id", "Booking Segment",
                                                   std::make_unique<BookingSegment>(), "booking_date");

    m_booking_status = createField<ChoiceField<std::string>>("booking_status", "Booking Status");
    m_booking_status->addChoice({"READY","READY"});
    m_booking_status->addChoice({"PLAYED","PLAYED"});
    m_booking_status->addChoice({"CANCEL","CANCEL"});

    m_play_date = createField<DateField>("play_date", "Play Date");

    m_play_time = createField<DateTimeField>("play_time", "Play Time");

    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
                                          std::make_unique<Spot>(), "name");

    m_audio = createField<ForeignKeyField>("audio_id", "Audio",
                                          std::make_unique<SpotAudio>(), "spot_id");

    mHeader << QString::fromStdString(schedule()->displayName());

    setTableName("rave_orderbooking");

}

OrderBooking::~OrderBooking()
{

}

ForeignKeyField *OrderBooking::booking_segment() const
{
    return m_booking_segment;
}

void OrderBooking::set_booking_segment(int val)
{
    m_booking_segment->setValue(val);
}

ForeignKeyField *OrderBooking::schedule() const
{
    return m_schedule;
}

void OrderBooking::set_schedule(int val)
{
    m_schedule->setValue(val);
}

ChoiceField<std::string>* OrderBooking::booking_status() const
{
    return m_booking_status;
}

void OrderBooking::set_booking_status(std::string val)
{
    m_booking_status->setValue(val);
}

DateField *OrderBooking::play_date() const
{
    return m_play_date;
}

void OrderBooking::set_play_date(QDate val)
{
    m_play_date->setValue(val);
}

DateTimeField *OrderBooking::play_time() const
{
    return m_play_time;
}

void OrderBooking::set_play_time(QDateTime val)
{
    m_play_time->setValue(val);
}

ForeignKeyField *OrderBooking::spot() const
{
    return m_spot;
}

void OrderBooking::set_spot(int val)
{
    m_spot->setValue(val);
}

ForeignKeyField *OrderBooking::audio() const
{
    return m_audio;
}

void OrderBooking::set_audio(int val)
{
    m_audio->setValue(val);
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
