#include "../framework/choicefield.h"
#include "timeband.h"

TimeBand::TimeBand()
{
    mName = createField<StringField>("name", "Band Name");
    mName->setMandatory(true);

    mLocked = createField<BooleanField>("locked", "is Band Locked");
    mLocked->setSearchable(false);

    mBillMethod = createField<ChoiceField<std::string>>("bill_method", "Bill Method");
    mBillMethod->setSearchable(false);
    mBillMethod->addChoice({"FF","Flat Fee"});
    mBillMethod->addChoice({"IP","Increment By Percentage"});
    mBillMethod->setValue("FF");

    mBasePrice  = createField<DecimalField>("base_price", "Base Price");
    mBasePrice->setSearchable(false);
    mBaseDuration  = createField<IntegerField>("base_duration", "Base Duration");
    mBaseDuration->setSearchable(false);

    mIPDuration1  = createField<IntegerField>("ip_duration1","");
    mIPDuration1->setSearchable(false);
    mPercent1  = createField<IntegerField>("ip_percent1","");
    mPercent1->setSearchable(false);

    mIPDuration2  = createField<IntegerField>("ip_duration2","");
    mIPDuration2->setSearchable(false);
    mPercent2  = createField<IntegerField>("ip_percent2","");
    mPercent2->setSearchable(false);
    mIPDuration3  = createField<IntegerField>("ip_duration3","");
    mIPDuration3->setSearchable(false);
    mPercent3  = createField<IntegerField>("ip_percent3","");
    mPercent3->setSearchable(false);
    mIPDuration4  = createField<IntegerField>("ip_duration4","");
    mIPDuration4->setSearchable(false);
    mPercent4  = createField<IntegerField>("ip_percent4","");
    mPercent4->setSearchable(false);
    mIPDuration5  = createField<IntegerField>("ip_duration5","");
    mIPDuration5->setSearchable(false);
    mPercent5  = createField<IntegerField>("ip_percent5","");
    mPercent5->setSearchable(false);
    mIPDuration6  = createField<IntegerField>("ip_duration6","");
    mIPDuration6->setSearchable(false);
    mPercent6  = createField<IntegerField>("ip_percent6","");
    mPercent6->setSearchable(false);
    mIPDuration7  = createField<IntegerField>("ip_duration7","");
    mIPDuration7->setSearchable(false);
    mPercent7  = createField<IntegerField>("ip_percent7","");
    mPercent7->setSearchable(false);
    mIPDuration8  = createField<IntegerField>("ip_duration8","");
    mIPDuration8->setSearchable(false);
    mPercent8  = createField<IntegerField>("ip_percent8","");
    mPercent8->setSearchable(false);
    mIPDuration9  = createField<IntegerField>("ip_duration9","");
    mIPDuration9->setSearchable(false);
    mPercent9  = createField<IntegerField>("ip_percent9","");
    mPercent9->setSearchable(false);
    mIPDuration10  = createField<IntegerField>("ip_duration10","");
    mIPDuration10->setSearchable(false);
    mPercent10  = createField<IntegerField>("ip_percent10","");
    mPercent10->setSearchable(false);
    mIPDuration11  = createField<IntegerField>("ip_duration11","");
    mIPDuration11->setSearchable(false);
    mPercent11  = createField<IntegerField>("ip_percent11","");
    mPercent11->setSearchable(false);
    mIPDuration12  = createField<IntegerField>("ip_duration12","");
    mIPDuration12->setSearchable(false);
    mPercent12  = createField<IntegerField>("ip_percent12","");
    mPercent12->setSearchable(false);

    mDaypart1  = createField<StringField>("daypart1", "");
    mDaypart1->setSearchable(false);
    mDaypart2  = createField<StringField>("daypart2", "");
    mDaypart2->setSearchable(false);
    mDaypart3  = createField<StringField>("daypart3", "");
    mDaypart3->setSearchable(false);
    mDaypart4  = createField<StringField>("daypart4", "");
    mDaypart4->setSearchable(false);
    mDaypart5  = createField<StringField>("daypart5", "");
    mDaypart5->setSearchable(false);
    mDaypart6  = createField<StringField>("daypart6", "");
    mDaypart6->setSearchable(false);
    mDaypart7  = createField<StringField>("daypart7", "");
    mDaypart7->setSearchable(false);

    mHeader << QString::fromStdString(mName->fieldLabel());

    setTableName("rave_timeband");

}

TimeBand::~TimeBand()
{
}

std::string TimeBand::tableName() const
{
    return mTableName;
}

void TimeBand::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> TimeBand::mapFields(StringMap* raw_entity)
{
    std::unique_ptr<TimeBand> timeBand = entityFieldMap<TimeBand>(raw_entity);
    return std::move(timeBand);
}

std::list<std::string> TimeBand::tableViewColumns()
{
    std::list<std::string> cols;

    cols.push_back(name()->displayName());

    return cols;
}

std::vector<std::string> TimeBand::tableViewValues()
{
    return { name()->displayName() };
}

QStringList TimeBand::tableHeaders() const
{
    return mHeader;
}

std::string TimeBand::searchColumn() const
{
    return name()->valueToString();
}

void TimeBand::populateEntity() {}

StringField* TimeBand::name() const
{
    return mName;
}

void TimeBand::setName(std::string val)
{
    mName->setValue( val );
}

BooleanField* TimeBand::locked()
{
    return mLocked;
}

void TimeBand::setLocked(bool val)
{
    mLocked->setValue( val );
}

ChoiceField<std::string>* TimeBand::billMethod()
{
    return mBillMethod;
}

void TimeBand::setBillMethod(const std::string val)
{
    mBillMethod->setValue( val );
}

IntegerField* TimeBand::baseDuration() const
{
    return mBaseDuration;
}
void TimeBand::setBaseDuration(int val)
{
    mBaseDuration->setValue( val );
}
DecimalField* TimeBand::basePrice() const
{
    return mBasePrice;
}
void TimeBand::setBasePrice(double val)
{
    mBasePrice->setValue( val );
}
StringField* TimeBand::daypart1() const
{
    return mDaypart1;
}
void TimeBand::setDaypart1(std::string val)
{
    mDaypart1->setValue( val );
}
StringField* TimeBand::daypart2() const
{
    return mDaypart2;
}
void TimeBand::setDaypart2(std::string val)
{
    mDaypart2->setValue( val );
}
StringField* TimeBand::daypart3() const
{
    return mDaypart3;
}
void TimeBand::setDaypart3(std::string val)
{
    mDaypart3->setValue( val );
}
StringField* TimeBand::daypart4() const
{
    return mDaypart4;
}
void TimeBand::setDaypart4(std::string val)
{
    mDaypart4->setValue( val );
}
StringField* TimeBand::daypart5() const
{
    return mDaypart5;
}
void TimeBand::setDaypart5(std::string val)
{
    mDaypart5->setValue( val );
}
StringField* TimeBand::daypart6() const
{
    return mDaypart6;
}
void TimeBand::setDaypart6(std::string val)
{
    mDaypart6->setValue( val );
}
StringField* TimeBand::daypart7() const
{
    return mDaypart7;
}
void TimeBand::setDaypart7(std::string val)
{
    mDaypart7->setValue( val );
}
IntegerField* TimeBand::iPDuration1() const
{
    return mIPDuration1;
}
void TimeBand::setIPDuration1(int val)
{
    mIPDuration1->setValue( val );
}
IntegerField* TimeBand::iPDuration10() const
{
    return mIPDuration10;
}
void TimeBand::setIPDuration10(int val)
{
    mIPDuration10->setValue( val );
}
IntegerField* TimeBand::iPDuration11() const
{
    return mIPDuration11;
}
void TimeBand::setIPDuration11(int val)
{
    mIPDuration11->setValue( val );
}
IntegerField* TimeBand::iPDuration12() const
{
    return mIPDuration12;
}
void TimeBand::setIPDuration12(int val)
{
    mIPDuration12->setValue( val );
}
IntegerField* TimeBand::iPDuration2() const
{
    return mIPDuration2;
}
void TimeBand::setIPDuration2(int val)
{
    mIPDuration2->setValue( val );
}
IntegerField* TimeBand::iPDuration3() const
{
    return mIPDuration3;
}
void TimeBand::setIPDuration3(int val)
{
    mIPDuration3->setValue( val );
}
IntegerField* TimeBand::iPDuration4() const
{
    return mIPDuration4;
}
void TimeBand::setIPDuration4(int val)
{
    mIPDuration4->setValue( val );
}
IntegerField* TimeBand::iPDuration5() const
{
    return mIPDuration5;
}
void TimeBand::setIPDuration5(int val)
{
    mIPDuration5->setValue( val );
}
IntegerField* TimeBand::iPDuration6() const
{
    return mIPDuration6;
}
void TimeBand::setIPDuration6(int val)
{
    mIPDuration6->setValue( val );
}
IntegerField* TimeBand::iPDuration7() const
{
    return mIPDuration7;
}
void TimeBand::setIPDuration7(int val)
{
    mIPDuration7->setValue( val );
}
IntegerField* TimeBand::iPDuration8() const
{
    return mIPDuration8;
}
void TimeBand::setIPDuration8(int val)
{
    mIPDuration8->setValue( val );
}
IntegerField* TimeBand::iPDuration9() const
{
    return mIPDuration9;
}
void TimeBand::setIPDuration9(int val)
{
    mIPDuration9->setValue( val );
}
IntegerField* TimeBand::percent1() const
{
    return mPercent1;
}
void TimeBand::setPercent1(int val)
{
    mPercent1->setValue( val );
}
IntegerField* TimeBand::percent10() const
{
    return mPercent10;
}
void TimeBand::setPercent10(int val)
{
    mPercent10->setValue( val );
}
IntegerField* TimeBand::percent11() const
{
    return mPercent11;
}
void TimeBand::setPercent11(int val)
{
    mPercent11->setValue( val );
}
IntegerField* TimeBand::percent12() const
{
    return mPercent12;
}
void TimeBand::setPercent12(int val)
{
    mPercent12->setValue( val );
}
IntegerField* TimeBand::percent2() const
{
    return mPercent2;
}
void TimeBand::setPercent2(int val)
{
    mPercent2->setValue( val );
}
IntegerField* TimeBand::percent3() const
{
    return mPercent3;
}
void TimeBand::setPercent3(int val)
{
    mPercent3->setValue( val );
}
IntegerField* TimeBand::percent4() const
{
    return mPercent4;
}
void TimeBand::setPercent4(int val)
{
    mPercent4->setValue( val );
}
IntegerField* TimeBand::percent5() const
{
    return mPercent5;
}
void TimeBand::setPercent5(int val)
{
    mPercent5->setValue( val );
}
IntegerField* TimeBand::percent6() const
{
    return mPercent6;
}
void TimeBand::setPercent6(int val)
{
    mPercent6->setValue( val );
}
IntegerField* TimeBand::percent7() const
{
    return mPercent7;
}
void TimeBand::setPercent7(int val)
{
    mPercent7->setValue( val );
}
IntegerField* TimeBand::percent8() const
{
    return mPercent8;
}
void TimeBand::setPercent8(int val)
{
    mPercent8->setValue( val );
}
IntegerField* TimeBand::percent9() const
{
    return mPercent9;
}
void TimeBand::setPercent9(int val)
{
    mPercent9->setValue( val );
}
