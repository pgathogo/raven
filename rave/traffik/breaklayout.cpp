#include "breaklayout.h"
#include "../framework/choicefield.h"

BreakLayout::BreakLayout()
{
    mName = createField<StringField>("name", "Name");
    mName->setMandatory(true);

    mDescription = createField<TextField>("description", "Description");
    mDescription->setSearchable(false);

    mWeekDays = createField<StringField>("week_days", "Week Days");
    mWeekDays->setSearchable(false);

    mTimeInterval = createField<ChoiceField<int>>("time_interval", "Time Interval");
    mTimeInterval->addChoice({15, "15 Mins"});
    mTimeInterval->addChoice({10, "10 Mins"});
    mTimeInterval->addChoice({5, "5 Mins"});
    mTimeInterval->addChoice({1, "1 Min"});

    mMonBit = createField<IntegerField>("Mon", "Monday");
    mMonBit->setFormOnly(true);
    mMonBit->setSearchable(false);

    mTueBit = createField<IntegerField>("Tue", "Tuesday");
    mTueBit->setFormOnly(true);
    mTueBit->setSearchable(false);

    mWedBit = createField<IntegerField>("Wed", "Wednesday");
    mWedBit->setFormOnly(true);
    mWedBit->setSearchable(false);

    mThuBit = createField<IntegerField>("Thur", "Thursday");
    mThuBit->setFormOnly(true);
    mThuBit->setSearchable(false);

    mFriBit = createField<IntegerField>("Fri", "Friday");
    mFriBit->setFormOnly(true);
    mFriBit->setSearchable(false);

    mSatBit = createField<IntegerField>("Sat", "Saturday");
    mSatBit->setFormOnly(true);
    mSatBit->setSearchable(false);

    mSunBit = createField<IntegerField>("Sun", "Sunday");
    mSunBit->setFormOnly(true);
    mSunBit->setSearchable(false);

    mHeader << stoq(mName->fieldLabel());
    setTableName("rave_breaklayout");
}

BreakLayout::~BreakLayout()
{
}

StringField *BreakLayout::name() const
{
    return mName;
}

void BreakLayout::setName(const std::string nm)
{
    mName->setValue( nm );
}

TextField *BreakLayout::description() const
{
    return mDescription;
}

void BreakLayout::setDescription(const std::string desc)
{
    mDescription->setValue( desc );
}

StringField *BreakLayout::weekDays() const
{
    return mWeekDays;
}

void BreakLayout::setWeekDays(const std::string wkdays)
{
    mWeekDays->setValue( wkdays );

}

ChoiceField<int> *BreakLayout::timeInterval()
{
    return mTimeInterval;
}

void BreakLayout::setTimeInterval(int ti)
{
    mTimeInterval->setValue( ti );
}

IntegerField *BreakLayout::monBit() const
{
    return mMonBit;
}

void BreakLayout::setMonBit(int bit)
{
    mMonBit->setValue( bit );
}

IntegerField *BreakLayout::tueBit() const
{
    return mTueBit;
}

void BreakLayout::setTueBit(int bit)
{
    mTueBit->setValue( bit );
}

IntegerField *BreakLayout::wedBit() const
{
    return mWedBit;
}

void BreakLayout::setWedBit(int bit)
{
    mWedBit->setValue( bit );
}

IntegerField *BreakLayout::thuBit() const
{
    return mThuBit;
}

void BreakLayout::setThuBit(int bit)
{
    mThuBit->setValue( bit );
}

IntegerField *BreakLayout::friBit() const
{
    return mFriBit;
}

void BreakLayout::setFriBit(int bit)
{
    mFriBit->setValue( bit );
}

IntegerField *BreakLayout::satBit() const
{
    return mSatBit;
}

void BreakLayout::setSatBit(int bit)
{
    mSatBit->setValue( bit );
}

IntegerField *BreakLayout::sunBit() const
{
    return mSunBit;
}

void BreakLayout::setSunBit(int bit)
{
    mSunBit->setValue( bit );
}

WeekDayBits BreakLayout::getAccessBit(std::string abit)
{
   std::string mon{abit.at(0)};
   std::string tue{abit.at(1)};
   std::string wed{abit.at(2)};
   std::string thu{abit.at(3)};
   std::string fri{abit.at(4)};
   std::string sat{abit.at(5)};
   std::string sun{abit.at(6)};

   WeekDayBits bits = std::make_tuple(
               std::atoi(mon.c_str()),
               std::atoi(tue.c_str()),
               std::atoi(wed.c_str()),
               std::atoi(thu.c_str()),
               std::atoi(fri.c_str()),
               std::atoi(sat.c_str()),
               std::atoi(sun.c_str())
               );
   return bits;

}

std::string BreakLayout::access_bit_tostring()
{
    return std::to_string(monBit()->value())+
            std::to_string(tueBit()->value())+
            std::to_string(wedBit()->value())+
            std::to_string(thuBit()->value())+
            std::to_string(friBit()->value())+
            std::to_string(satBit()->value())+
            std::to_string(sunBit()->value());
}

std::string BreakLayout::tableName() const
{
    return mTableName;
}

void BreakLayout::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> BreakLayout::mapFields(StringMap *raw_entity)
{
}

std::list<std::string> BreakLayout::tableViewColumns() const
{
    std::list<std::string> cols;
    cols.push_back(name()->displayName());
    return cols;
}

std::vector<std::string> BreakLayout::tableViewValues()
{
    return { name()->displayName() };
}

QStringList BreakLayout::tableHeaders() const
{
    return mHeader;
}

std::string BreakLayout::searchColumn() const
{
    return name()->valueToString();
}

void BreakLayout::populateEntity()
{
}

std::unique_ptr<BaseEntity> BreakLayout::cloneAsUnique()
{
    return std::make_unique<BreakLayout>();
}

void BreakLayout::afterMapping(BaseEntity &entity)
{
}
