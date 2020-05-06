#include <QStandardItem>
#include "voiceover.h"
#include "typeexclusion.h"
#include "../framework/manytomany.h"

VoiceOver::VoiceOver()
    :BaseEntity{}
    ,mName{}
    ,mVoiceEx{}
{
    mName = createField<StringField>("voiceover_name", "Voice Over Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);

    mMobileNo = createField<StringField>("mobile_no", "Mobile No");

    mGender = createField<LookupField>("gender", "Gender");
    mGender->setDBColumnName("gender_id");

    mDaypart1 = createField<StringField>("daypart1", "Daypart1");
    mDaypart1->setSearchable(false);

    mDaypart2 = createField<StringField>("daypart2", "Daypart2");
    mDaypart2->setSearchable(false);

    mDaypart3 = createField<StringField>("daypart3", "Daypart3");
    mDaypart3->setSearchable(false);

    mDaypart4 = createField<StringField>("daypart4", "Daypart4");
    mDaypart4->setSearchable(false);
    mDaypart5 = createField<StringField>("daypart5", "Daypart5");
    mDaypart5->setSearchable(false);
    mDaypart6 = createField<StringField>("daypart6", "Daypart6");
    mDaypart6->setSearchable(false);
    mDaypart7 = createField<StringField>("daypart7", "Daypart6");
    mDaypart7->setSearchable(false);

    mVoiceEx = new VoiceExclusion(this, new TypeExclusion);

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mGender->fieldLabel());

    setTableName("rave_voiceover");
}

VoiceOver::~VoiceOver()
{
}

std::string VoiceOver::tableName() const
{
    return mTableName;
}

void VoiceOver::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

BaseEntity* VoiceOver::copy() const
{
    return new VoiceOver;
}
std::unique_ptr<BaseEntity> VoiceOver::mapFields(StringMap* e)
{
    auto te = entityFieldMap<VoiceOver>(e);
    return std::move(te);
}

QList<QStandardItem*> VoiceOver::tableViewColumns()
{
    QString nm  = QString::fromStdString(name()->valueToString());
    QString gen = QString::fromStdString(gender()->valueToString());

    QStandardItem* Qname = new QStandardItem(nm);
    QStandardItem* Qgen = new QStandardItem(gen);
    return{Qname, Qgen};
}
std::vector<std::string> VoiceOver::tableViewValues()
{
    std::string gname  = name()->valueToString();
    std::string gen = gender()->valueToString();
    return{gname, gen};
}

QStringList VoiceOver::tableHeaders() const
{
    return mHeader;
}

std::string VoiceOver::searchColumn() const
{
    return name()->valueToString();
}

void VoiceOver::populateEntity()
{
    mName->setValueFromWidget();
    mGender->setValueFromWidget();
}

StringField* VoiceOver::name() const
{
    return mName;
}

void VoiceOver::setName(std::string aName)
{
    mName->setValue(aName);
}

StringField* VoiceOver::mobileno() const
{
    return mMobileNo;
}

void VoiceOver::setMobileNo(std::string mobileno)
{
    mMobileNo->setValue(mobileno);
}

LookupField* VoiceOver::gender() const
{
    return mGender;
}

void VoiceOver::setGender(int index)
{
    mGender->setIndex(index);
    // set curText
    // set mValue - id of selected record
}

StringField* VoiceOver::daypart1() const
{
    return mDaypart1;
}
void VoiceOver::setDaypart1(std::string dp)
{
    mDaypart1->setValue(dp);
}

StringField* VoiceOver::daypart2() const
{
    return mDaypart2;
}
void VoiceOver::setDaypart2(std::string dp)
{
    mDaypart2->setValue(dp);
}

StringField* VoiceOver::daypart3() const
{
    return mDaypart3;
}
void VoiceOver::setDaypart3(std::string dp)
{
    mDaypart3->setValue(dp);
}

StringField* VoiceOver::daypart4() const
{
    return mDaypart4;
}
void VoiceOver::setDaypart4(std::string dp)
{
    mDaypart4->setValue(dp);
}

StringField* VoiceOver::daypart5() const
{
    return mDaypart5;
}
void VoiceOver::setDaypart5(std::string dp)
{
    mDaypart5->setValue(dp);
}

StringField* VoiceOver::daypart6() const
{
    return mDaypart6;
}
void VoiceOver::setDaypart6(std::string dp)
{
    mDaypart6->setValue(dp);
}

StringField* VoiceOver::daypart7() const
{
    return mDaypart7;
}
void VoiceOver::setDaypart7(std::string dp)
{
    mDaypart7->setValue(dp);
}

