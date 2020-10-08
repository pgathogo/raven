#include "bookingsegment.h"
#include "order.h"
#include "spot.h"

BookingSegment::BookingSegment()
{
     mOrder = createField<ForeignKeyField>("order_id", "Order",
                                           std::make_unique<Order>(), "title");
    mSpot = createField<ForeignKeyField>("spot_id", "Spot",
                                         std::make_unique<Spot>(), "name");
    mBookingDate = createField<DateField>("booking_date", "Booking Date");
    mStartDate = createField<DateField>("start_date", "Start Date");
    mEndDate = createField<DateField>("end_date", "End Date");
    mBookingCount = createField<IntegerField>("booking_count", "Booking Count");
    mSegmentValue = createField<DecimalField>("segment_value", "Segment Value");
    mComments = createField<TextField>("comments", "Comments");
    mAddLogin = createField<StringField>("add_login", "Add Login");
    mAddDateTime = createField<DateTimeField>("add_dtime", "Add Date Time");

    mHeader << QString::fromStdString(order()->fieldLabel())
            << QString::fromStdString(spot()->fieldLabel())
            << QString::fromStdString(bookingCount()->fieldLabel());

    setTableName("rave_bookingsegment");

}

BookingSegment::~BookingSegment()
{
}

ForeignKeyField *BookingSegment::order() const
{
    return mOrder;
}

void BookingSegment::setOrder(int val)
{
    mOrder->setValue(val);
}

ForeignKeyField *BookingSegment::spot() const
{
    return mSpot;
}

void BookingSegment::setSpot(int val)
{
    mSpot->setValue(val);
}

DateField *BookingSegment::bookingDate() const
{
    return mBookingDate;
}

void BookingSegment::setBookingDate(QDate val)
{
    mBookingDate->setValue(val);
}

DateField *BookingSegment::startDate() const
{
    return mStartDate;
}

void BookingSegment::setStartDate(QDate val)
{
    mStartDate->setValue(val);
}

DateField *BookingSegment::endDate() const
{
    return mEndDate;
}

void BookingSegment::setEndDate(QDate val)
{
    mEndDate->setValue(val);
}

IntegerField *BookingSegment::bookingCount() const
{
    return mBookingCount;
}

void BookingSegment::setBookingCount(int val)
{
    mBookingCount->setValue(val);
}

DecimalField *BookingSegment::segmentValue() const
{
   return mSegmentValue;
}

void BookingSegment::setSegmentValue(double val)
{
    mSegmentValue->setValue(val);
}

TextField *BookingSegment::comments() const
{
    return mComments;
}

void BookingSegment::setComments(const std::string val)
{
    mComments->setValue(val);
}

StringField *BookingSegment::addLogin() const
{
    return mAddLogin;
}

void BookingSegment::setAddLogin(const std::string val)
{
    mAddLogin->setValue(val);
}

DateTimeField *BookingSegment::addDateTime() const
{
    return mAddDateTime;
}

void BookingSegment::setAddDateTime(QDateTime val)
{
    mAddDateTime->setValue(val);
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
}

std::vector<std::string> BookingSegment::tableViewColumns() const
{
    return tableViewCols<std::string>(
               order()->displayName(),
               spot()->displayName(),
               bookingCount()->displayName()
                );

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

std::unique_ptr<BaseEntity> BookingSegment::cloneAsUnique()
{
    return std::make_unique<BookingSegment>();
}

void BookingSegment::afterMapping(BaseEntity &entity)
{
}
