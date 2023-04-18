#include "bookingsegment.h"
#include "order.h"
#include "spot.h"

BookingSegment::BookingSegment()
{
    m_order = createField<ForeignKeyField>("order_id", "Order",
                                           std::make_unique<Order>(), "title");

    m_booking_date = createField<DateField>("booking_date", "Booking Date");
    m_start_date = createField<DateField>("start_date", "Start Date");
    m_end_date = createField<DateField>("end_date", "End Date");

    m_booking_count = createField<IntegerField>("booking_count", "Booking Count");
    m_segment_value = createField<DecimalField>("segment_value", "Segment Value");
    m_comments = createField<TextField>("comments", "Comments");
    m_add_login = createField<StringField>("add_login", "Add Login");

    m_add_date_time = createField<DateTimeField>("add_dtime", "Add Date Time");

    mHeader << stoq(order()->fieldLabel())
            << stoq(booking_count()->fieldLabel());

    setTableName("rave_bookingsegment");

}

BookingSegment::~BookingSegment()
{
}

ForeignKeyField* BookingSegment::order() const
{
    return m_order;
}

void BookingSegment::set_order(int val)
{
    m_order->setValue(val);
}

DateField *BookingSegment::booking_date() const
{
    return m_booking_date;
}

void BookingSegment::set_booking_date(QDate val)
{
    m_booking_date->setValue(val);
}

DateField *BookingSegment::start_date() const
{
    return m_start_date;
}

void BookingSegment::set_start_date(QDate val)
{
    m_start_date->setValue(val);
}

DateField *BookingSegment::end_date() const
{
    return m_end_date;
}

void BookingSegment::set_end_date(QDate val)
{
    m_end_date->setValue(val);
}

IntegerField *BookingSegment::booking_count() const
{
    return m_booking_count;
}

void BookingSegment::set_booking_count(int val)
{
    m_booking_count->setValue(val);
}

DecimalField *BookingSegment::segment_value() const
{
   return m_segment_value;
}

void BookingSegment::set_segment_value(double val)
{
    m_segment_value->setValue(val);
}

TextField *BookingSegment::comments() const
{
    return m_comments;
}

void BookingSegment::set_comments(const std::string val)
{
    m_comments->setValue(val);
}

StringField *BookingSegment::add_login() const
{
    return m_add_login;
}

void BookingSegment::set_add_login(const std::string val)
{
    m_add_login->setValue(val);
}

DateTimeField *BookingSegment::add_date_time() const
{
    return m_add_date_time;
}

void BookingSegment::set_add_date_time(QDateTime val)
{
    m_add_date_time->setValue(val);
}

std::string BookingSegment::tableName() const
{
    return mTableName;
}

void BookingSegment::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> BookingSegment::mapFields(StringMap *raw_entity)
{
    return nullptr;
}

std::vector<std::string> BookingSegment::tableViewColumns() const
{
    return tableViewCols<std::string>(
               order()->displayName(),
               booking_count()->displayName());
}

std::vector<std::string> BookingSegment::tableViewValues()
{
    return tableViewColumns();
}

QStringList BookingSegment::tableHeaders() const
{
    return mHeader;
}

std::string BookingSegment::searchColumn() const
{
    return order()->valueToString();
}

void BookingSegment::populateEntity()
{
}

std::shared_ptr<BaseEntity> BookingSegment::cloneAsShared()
{
    return std::make_shared<BookingSegment>();
}

void BookingSegment::afterMapping(BaseEntity &entity)
{
}
