
#include "breaklayout.h"
#include "breaklayoutline.h"

BreakLayoutLine::BreakLayoutLine()
{
    mBreakLayout = createField<ForeignKeyField>("break_layout_id", "Break Layout",
                                                std::make_unique<BreakLayout>(), "name");
    mWeekDay = createField<IntegerField>("week_day", "Week Day");
    mBreakTime = createField<TimeField>("break_time", "Break Time");
    mBreakHour = createField<IntegerField>("break_hour", "Break Hour");
    mDuration = createField<IntegerField>("duration", "Duration");
    mMaxSpots = createField<IntegerField>("max_spots", "Max Spots");

    mRowId = createField<IntegerField>("rowid", "RowId");
    mRowId->setFormOnly(true);

    mHeader << stoq(mBreakTime->fieldLabel())
            << stoq(mDuration->fieldLabel())
            << stoq(mMaxSpots->fieldLabel());

    setTableName("rave_breaklayoutline");
}

BreakLayoutLine::~BreakLayoutLine()
{
}

ForeignKeyField *BreakLayoutLine::breakLayout() const
{
    return mBreakLayout;
}

void BreakLayoutLine::setBreakLayout(int i)
{
    mBreakLayout->setValue(i);
}

IntegerField *BreakLayoutLine::weekDay() const
{
    return mWeekDay;
}

void BreakLayoutLine::setWeekDay(int wd)
{
    mWeekDay->setValue( wd );
}

TimeField *BreakLayoutLine::breakTime() const
{
    return mBreakTime;
}

void BreakLayoutLine::setBreakTime(QTime bt)
{
    mBreakTime->setValue( bt );
}

IntegerField *BreakLayoutLine::breakHour() const
{
    return mBreakHour;
}

void BreakLayoutLine::setBreakHour(int hr)
{
    mBreakHour->setValue( hr );
}

IntegerField *BreakLayoutLine::duration() const
{
    return mDuration;
}

void BreakLayoutLine::setDuration(int dur)
{
    mDuration->setValue( dur );
}

IntegerField *BreakLayoutLine::maxSpots() const
{
    return mMaxSpots;
}

void BreakLayoutLine::setMaxSpots(int ms)
{
    mMaxSpots->setValue( ms );
}

IntegerField *BreakLayoutLine::rowId() const
{
    return mRowId;
}

void BreakLayoutLine::setRowId(int id)
{
    mRowId->setValue( id );
}

std::string BreakLayoutLine::tableName() const
{
    return mTableName;
}

void BreakLayoutLine::setTableName(const std::string table_name)
{
    mTableName =  table_name;
}

std::unique_ptr<BaseEntity> BreakLayoutLine::mapFields(StringMap *raw_entity)
{
    return nullptr;
}

std::vector<std::string> BreakLayoutLine::tableViewColumns() const
{
    return tableViewCols<std::string>(
                breakTime()->displayName(),
                duration()->displayName(),
                maxSpots()->displayName()
                );
}

std::vector<std::string> BreakLayoutLine::tableViewValues()
{
    return tableViewColumns();
}

QStringList BreakLayoutLine::tableHeaders() const
{
    return mHeader;
}

std::string BreakLayoutLine::searchColumn() const
{
    return breakTime()->valueToString();
}

std::unique_ptr<BaseEntity> BreakLayoutLine::cloneAsUnique()
{
    return std::make_unique<BreakLayoutLine>();
}

void BreakLayoutLine::afterMapping(BaseEntity &entity)
{
}
