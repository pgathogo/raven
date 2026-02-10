#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/schedule.h"
#include "../../../rave/audio/audio.h"
#include "order.h"
#include "orderbooking.h"
#include "bookingsegment.h"
#include "spotaudio.h"
#include "spot.h"
#include "advertmedia.h"

#include "reasonvoidbooking.h"

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
    m_booking_status->addChoice({"SKIPPED","SKIPPED"});

    m_play_date = createField<DateField>("play_date", "Play Date");
    m_play_time = createField<DateTimeField>("play_time", "Play Time");

    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
                                          std::make_unique<TRAFFIK::Spot>(), "name");

    m_spot_audio = createField<ForeignKeyField>("booked_audio_id", "Audio",
                                          std::make_unique<PIXELPLAN::AdvertMedia>(), "id");

    m_played_audio = createField<IntegerField>("played_audio", "Played Audio ID");

    m_book_date = createField<DateField>("book_date", "Book Date");
    m_book_time = createField<TimeField>("book_time", "Book Time");
    m_book_hour = createField<IntegerField>("book_hour", "Book Hour");

    m_book_seq = createField<IntegerField>("book_seq", "Book Sequence");

    m_order = createField<ForeignKeyField>("order_id", "Order ID",
                                           std::make_unique<Order>(), "order_number");

    m_void_reason = createField<ForeignKeyField>("void_reason_id", "Void Reaason",
                                                 std::make_unique<PIXELPLAN::ReasonVoidBooking>(), "reason");
    m_comment = createField<TextField>("comments", "Comment");
    m_add_login = createField<StringField>("add_login" , "Add login");
    m_add_dtime = createField<DateTimeField>("add_dtime", "Add Date Time");

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

ForeignKeyField* OrderBooking::order() const
{
    return m_order;
}

void OrderBooking::set_order(int order_id)
{
   m_order->setValue(order_id);

}

ForeignKeyField* OrderBooking::void_reason()
{
    return m_void_reason;
}

void OrderBooking::set_void_reason(int reason)
{
    m_void_reason->setValue(reason);

}

TextField* OrderBooking::comment()
{
    return m_comment;

}

void OrderBooking::set_comment(const std::string comment)
{
    m_comment->setValue(comment);

}

StringField* OrderBooking::add_login()
{
    return m_add_login;

}


void OrderBooking::set_add_login(const std::string login)
{
    m_add_login->setValue(login);

}

DateTimeField* OrderBooking::add_dtime()
{
    return m_add_dtime;

}

void OrderBooking::set_add_dtime(QDateTime dtime)
{
    m_add_dtime->setValue(dtime);

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
