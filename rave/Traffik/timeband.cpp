#include "../framework/choicefield.h"
#include "timeband.h"

TimeBand::TimeBand()
{
    m_name = createField<StringField>("name", "Band Name");
    m_name->setMandatory(true);

    m_locked = createField<BooleanField>("locked", "is Band Locked");
    m_locked->setSearchable(false);

    m_bill_method = createField<ChoiceField<std::string>>("bill_method", "Bill Method");
    m_bill_method->setSearchable(false);
    m_bill_method->addChoice({"FF","Flat Fee"});
    m_bill_method->addChoice({"IP","Increment By Percentage"});
    m_bill_method->setValue("FF");

    m_base_price  = createField<DecimalField>("base_price", "Base Price");
    m_base_price->setSearchable(false);
    m_base_duration  = createField<IntegerField>("base_duration", "Base Duration");
    m_base_duration->setSearchable(false);

    m_ip_duration1  = createField<IntegerField>("ip_duration1","");
    m_ip_duration1->setSearchable(false);
    m_percent1  = createField<IntegerField>("ip_percent1","");
    m_percent1->setSearchable(false);

    m_ip_duration2  = createField<IntegerField>("ip_duration2","");
    m_ip_duration2->setSearchable(false);
    m_percent2  = createField<IntegerField>("ip_percent2","");
    m_percent2->setSearchable(false);
    m_ip_duration3  = createField<IntegerField>("ip_duration3","");
    m_ip_duration3->setSearchable(false);
    m_percent3  = createField<IntegerField>("ip_percent3","");
    m_percent3->setSearchable(false);
    m_ip_duration4  = createField<IntegerField>("ip_duration4","");
    m_ip_duration4->setSearchable(false);
    m_percent4  = createField<IntegerField>("ip_percent4","");
    m_percent4->setSearchable(false);
    m_ip_duration5  = createField<IntegerField>("ip_duration5","");
    m_ip_duration5->setSearchable(false);
    m_percent5  = createField<IntegerField>("ip_percent5","");
    m_percent5->setSearchable(false);
    m_ip_duration6  = createField<IntegerField>("ip_duration6","");
    m_ip_duration6->setSearchable(false);
    m_percent6  = createField<IntegerField>("ip_percent6","");
    m_percent6->setSearchable(false);
    m_ip_duration7  = createField<IntegerField>("ip_duration7","");
    m_ip_duration7->setSearchable(false);
    m_percent7  = createField<IntegerField>("ip_percent7","");
    m_percent7->setSearchable(false);
    m_ip_duration8  = createField<IntegerField>("ip_duration8","");
    m_ip_duration8->setSearchable(false);
    m_percent8  = createField<IntegerField>("ip_percent8","");
    m_percent8->setSearchable(false);
    m_ip_duration9  = createField<IntegerField>("ip_duration9","");
    m_ip_duration9->setSearchable(false);
    m_percent9  = createField<IntegerField>("ip_percent9","");
    m_percent9->setSearchable(false);
    m_ip_duration10  = createField<IntegerField>("ip_duration10","");
    m_ip_duration10->setSearchable(false);
    m_percent10  = createField<IntegerField>("ip_percent10","");
    m_percent10->setSearchable(false);
    m_ip_duration11  = createField<IntegerField>("ip_duration11","");
    m_ip_duration11->setSearchable(false);
    m_percent11  = createField<IntegerField>("ip_percent11","");
    m_percent11->setSearchable(false);
    m_ip_duration12  = createField<IntegerField>("ip_duration12","");
    m_ip_duration12->setSearchable(false);
    m_percent12  = createField<IntegerField>("ip_percent12","");
    m_percent12->setSearchable(false);

    m_daypart1  = createField<StringField>("daypart1", "");
    m_daypart1->setSearchable(false);
    m_daypart2  = createField<StringField>("daypart2", "");
    m_daypart2->setSearchable(false);
    m_daypart3  = createField<StringField>("daypart3", "");
    m_daypart3->setSearchable(false);
    m_daypart4  = createField<StringField>("daypart4", "");
    m_daypart4->setSearchable(false);
    m_daypart5  = createField<StringField>("daypart5", "");
    m_daypart5->setSearchable(false);
    m_daypart6  = createField<StringField>("daypart6", "");
    m_daypart6->setSearchable(false);
    m_daypart7  = createField<StringField>("daypart7", "");
    m_daypart7->setSearchable(false);

    mHeader << QString::fromStdString(m_name->fieldLabel());

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

std::vector<std::string> TimeBand::tableViewColumns() const
{
    return tableViewCols<std::string>(name()->displayName());
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
    return m_name;
}

void TimeBand::setName(std::string val)
{
    m_name->setValue( val );
}

BooleanField* TimeBand::locked()
{
    return m_locked;
}

void TimeBand::setLocked(bool val)
{
    m_locked->setValue( val );
}

ChoiceField<std::string>* TimeBand::billMethod()
{
    return m_bill_method;
}

void TimeBand::setBillMethod(const std::string val)
{
    m_bill_method->setValue( val );
}

IntegerField* TimeBand::baseDuration() const
{
    return m_base_duration;
}
void TimeBand::setBaseDuration(int val)
{
    m_base_duration->setValue( val );
}
DecimalField* TimeBand::basePrice() const
{
    return m_base_price;
}
void TimeBand::setBasePrice(double val)
{
    m_base_price->setValue( val );
}
StringField* TimeBand::daypart1() const
{
    return m_daypart1;
}
void TimeBand::setDaypart1(std::string val)
{
    m_daypart1->setValue( val );
}
StringField* TimeBand::daypart2() const
{
    return m_daypart2;
}
void TimeBand::setDaypart2(std::string val)
{
    m_daypart2->setValue( val );
}
StringField* TimeBand::daypart3() const
{
    return m_daypart3;
}
void TimeBand::setDaypart3(std::string val)
{
    m_daypart3->setValue( val );
}
StringField* TimeBand::daypart4() const
{
    return m_daypart4;
}
void TimeBand::setDaypart4(std::string val)
{
    m_daypart4->setValue( val );
}
StringField* TimeBand::daypart5() const
{
    return m_daypart5;
}
void TimeBand::setDaypart5(std::string val)
{
    m_daypart5->setValue( val );
}
StringField* TimeBand::daypart6() const
{
    return m_daypart6;
}
void TimeBand::setDaypart6(std::string val)
{
    m_daypart6->setValue( val );
}
StringField* TimeBand::daypart7() const
{
    return m_daypart7;
}
void TimeBand::setDaypart7(std::string val)
{
    m_daypart7->setValue( val );
}
IntegerField* TimeBand::iPDuration1() const
{
    return m_ip_duration1;
}
void TimeBand::setIPDuration1(int val)
{
    m_ip_duration1->setValue( val );
}
IntegerField* TimeBand::iPDuration10() const
{
    return m_ip_duration10;
}
void TimeBand::setIPDuration10(int val)
{
    m_ip_duration10->setValue( val );
}
IntegerField* TimeBand::iPDuration11() const
{
    return m_ip_duration11;
}
void TimeBand::setIPDuration11(int val)
{
    m_ip_duration11->setValue( val );
}
IntegerField* TimeBand::iPDuration12() const
{
    return m_ip_duration12;
}
void TimeBand::setIPDuration12(int val)
{
    m_ip_duration12->setValue( val );
}
IntegerField* TimeBand::iPDuration2() const
{
    return m_ip_duration2;
}
void TimeBand::setIPDuration2(int val)
{
    m_ip_duration2->setValue( val );
}
IntegerField* TimeBand::iPDuration3() const
{
    return m_ip_duration3;
}
void TimeBand::setIPDuration3(int val)
{
    m_ip_duration3->setValue( val );
}
IntegerField* TimeBand::iPDuration4() const
{
    return m_ip_duration4;
}
void TimeBand::setIPDuration4(int val)
{
    m_ip_duration4->setValue( val );
}
IntegerField* TimeBand::iPDuration5() const
{
    return m_ip_duration5;
}
void TimeBand::setIPDuration5(int val)
{
    m_ip_duration5->setValue( val );
}
IntegerField* TimeBand::iPDuration6() const
{
    return m_ip_duration6;
}
void TimeBand::setIPDuration6(int val)
{
    m_ip_duration6->setValue( val );
}
IntegerField* TimeBand::iPDuration7() const
{
    return m_ip_duration7;
}
void TimeBand::setIPDuration7(int val)
{
    m_ip_duration7->setValue( val );
}
IntegerField* TimeBand::iPDuration8() const
{
    return m_ip_duration8;
}
void TimeBand::setIPDuration8(int val)
{
    m_ip_duration8->setValue( val );
}
IntegerField* TimeBand::iPDuration9() const
{
    return m_ip_duration9;
}
void TimeBand::setIPDuration9(int val)
{
    m_ip_duration9->setValue( val );
}
IntegerField* TimeBand::percent1() const
{
    return m_percent1;
}
void TimeBand::setPercent1(int val)
{
    m_percent1->setValue( val );
}
IntegerField* TimeBand::percent10() const
{
    return m_percent10;
}
void TimeBand::setPercent10(int val)
{
    m_percent10->setValue( val );
}
IntegerField* TimeBand::percent11() const
{
    return m_percent11;
}
void TimeBand::setPercent11(int val)
{
    m_percent11->setValue( val );
}
IntegerField* TimeBand::percent12() const
{
    return m_percent12;
}
void TimeBand::setPercent12(int val)
{
    m_percent12->setValue( val );
}
IntegerField* TimeBand::percent2() const
{
    return m_percent2;
}
void TimeBand::setPercent2(int val)
{
    m_percent2->setValue( val );
}
IntegerField* TimeBand::percent3() const
{
    return m_percent3;
}
void TimeBand::setPercent3(int val)
{
    m_percent3->setValue( val );
}
IntegerField* TimeBand::percent4() const
{
    return m_percent4;
}
void TimeBand::setPercent4(int val)
{
    m_percent4->setValue( val );
}
IntegerField* TimeBand::percent5() const
{
    return m_percent5;
}
void TimeBand::setPercent5(int val)
{
    m_percent5->setValue( val );
}
IntegerField* TimeBand::percent6() const
{
    return m_percent6;
}
void TimeBand::setPercent6(int val)
{
    m_percent6->setValue( val );
}
IntegerField* TimeBand::percent7() const
{
    return m_percent7;
}
void TimeBand::setPercent7(int val)
{
    m_percent7->setValue( val );
}
IntegerField* TimeBand::percent8() const
{
    return m_percent8;
}
void TimeBand::setPercent8(int val)
{
    m_percent8->setValue( val );
}
IntegerField* TimeBand::percent9() const
{
    return m_percent9;
}
void TimeBand::setPercent9(int val)
{
    m_percent9->setValue( val );
}

std::unique_ptr<BaseEntity> TimeBand::cloneAsUnique()
{
    return std::make_unique<TimeBand>();
}

void TimeBand::afterMapping(BaseEntity &entity)
{
}

std::map<int, std::string> TimeBand::dayparts()
{
    std::map<int, std::string> dp;

    dp[1] = daypart1()->value();
    dp[2] = daypart2()->value();
    dp[3] = daypart3()->value();
    dp[4] = daypart4()->value();
    dp[5] = daypart5()->value();
    dp[6] = daypart6()->value();
    dp[7] = daypart7()->value();

    return dp;
}

std::vector<int> TimeBand::hours_in_dow(int dow)
{
    auto all_dayparts = dayparts();
    return daypart_to_hours(all_dayparts[dow]);
}

std::vector<int> TimeBand::daypart_to_hours(const std::string& dp_str)
{
    int hr = 0;
    std::vector<int> hrs;
    for(std::string::const_iterator it=dp_str.begin(); it != dp_str.end(); ++it){
        if (*it=='1')
            hrs.push_back(hr);
        ++hr;
    }

    return hrs;
}



