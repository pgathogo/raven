#include <QStandardItem>
#include "voiceover.h"
#include "../framework/manytomany.h"
#include "../framework/valuelist.h"
#include "../framework/entitydatamodel.h"

VoiceOver::VoiceOver()
    :BaseEntity{}
    ,mName{}
    ,mMobileNo{}
    ,mGender{}
    ,mVoiceEx{}
{

    mName = createField<StringField>("voiceover_name", "Voice Over Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);

    mMobileNo = createField<StringField>("mobile_no", "Mobile No");

    mGender = createField<ForeignKeyField>("gender", "Gender",
                                           std::make_unique<Gender>(), "value");
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

    mTypeEx = std::make_unique<TRAFFIK::TypeExclusion>();
    mVoiceEx = std::make_unique<VoiceExclusion>(this, mTypeEx.get());

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mGender->fieldLabel());

    setTableName("rave_voiceover");

}

VoiceOver::~VoiceOver()
{
    //delete mGenderModel;
    qDebug() << "VoiceOver::dtor";
}

std::string VoiceOver::tableName() const
{
    return mTableName;
}

void VoiceOver::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> VoiceOver::mapFields(StringMap* e)
{
    /*
    std::unique_ptr<VoiceOver> vo = entityFieldMap<VoiceOver>(e);
    vo->voiceEx().setParentId(vo->id());
    return std::move(vo);
    */
    return nullptr;

}

std::vector<std::string> VoiceOver::tableViewColumns() const
{

    return tableViewCols<std::string>(
                name()->displayName(),
                gender()->displayName()
                );
}

std::vector<std::string> VoiceOver::tableViewValues()
{
    return tableViewColumns();
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

ForeignKeyField* VoiceOver::gender() const
{
    return mGender;
}

void VoiceOver::setGender(int index)
{
    mGender->setIndex(index);
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


VoiceExclusion& VoiceOver::voiceEx()
{
    return *mVoiceEx;
}


std::shared_ptr<BaseEntity> VoiceOver::cloneAsShared()
{
    return std::make_shared<VoiceOver>();
}

void VoiceOver::afterMapping(BaseEntity &entity)
{
    mVoiceEx->setParentId(entity.id());
}
