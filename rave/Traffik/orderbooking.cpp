#include "../framework/choicefield.h"
#include "../framework/schedule.h"
#include "../audio/audio.h"
#include "orderbooking.h"
#include "bookingsegment.h"
#include "spotaudio.h"
#include "spot.h"

OrderBooking::OrderBooking()
    :m_booking_segment{ nullptr }
    ,m_schedule{ nullptr }
    ,m_booking_status{ nullptr }
    ,m_play_date{ nullptr }
    ,m_play_time{ nullptr }
    ,m_spot{ nullptr }
    ,m_spot_audio{ nullptr }
{
    m_schedule = createField<ForeignKeyField>("schedule_id", "Schedule",
                                             std::make_unique<Schedule>(), "schedule_date");

    m_booking_segment = createField<ForeignKeyField>("bookingsegment_id", "Booking Segment",
                                                   std::make_unique<BookingSegment>(), "booking_date");

    m_booking_status = createField<ChoiceField<std::string>>("booking_status", "Booking Status");
    m_booking_status->addChoice({"READY","READY"});
    m_booking_status->addChoice({"PLAYED","PLAYED"});
    m_booking_status->addChoice({"CANCELLED","CANCEL"});

    m_play_date = createField<DateField>("play_date", "Play Date");
    m_play_time = createField<DateTimeField>("play_time", "Play Time");

    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
                                          std::make_unique<TRAFFIK::Spot>(), "name");

    m_spot_audio = createField<ForeignKeyField>("booked_audio_id", "Audio",
                                          std::make_unique<AUDIO::Audio>(), "id");

    m_played_audio = createField<IntegerField>("played_audio", "Played Audio ID");

    m_book_date = createField<DateField>("book_date", "Book Date");
    m_book_time = createField<TimeField>("book_time", "Book Time");
    m_book_hour = createField<IntegerField>("book_hour", "Book Hour");

    m_book_seq = createField<IntegerField>("book_seq", "Book Sequence");

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

DateField* OrderBooking::book_date()
{
    return m_book_date;
}

void OrderBooking::set_book_date(QDate val)
{
    m_book_date->setValue(val);
}

TimeField* OrderBooking::book_time()
{
    return m_book_time;
}

void OrderBooking::set_book_time(QTime val)
{
    m_book_time->setValue(val);
}

IntegerField* OrderBooking::book_hour()
{
    return m_book_hour;
}

void OrderBooking::set_book_hour(int val)
{
    m_book_hour->setValue(val);
}

IntegerField* OrderBooking::book_seq()
{
    return m_book_seq;

}

void OrderBooking::set_book_seq(int seq)
{
    m_book_seq->setValue(seq);
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
    return m_spot_audio;
}

void OrderBooking::set_audio(int val)
{
    m_spot_audio->setValue(val);
}

IntegerField* OrderBooking::played_audio()
{
    return  m_played_audio;
}

void OrderBooking::set_played_audio(int audio_id)
{
    m_played_audio->setValue(audio_id);
}

std::string OrderBooking::tableName() const
{
    return mTableName;
}

void OrderBooking::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> OrderBooking::mapFields(StringMap* /*raw_entity*/)
{
    return nullptr;
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

std::shared_ptr<BaseEntity> OrderBooking::cloneAsShared()
{
    return std::make_shared<OrderBooking>();
}

void OrderBooking::afterMapping(BaseEntity &entity)
{
}
