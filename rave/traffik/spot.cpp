#include "spot.h"
#include "client.h"
#include "brand.h"
#include "../framework/entitydatamodel.h"

#include "voiceover.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"
#include "typeexclusion.h"

Spot::Spot()
    :mName{}
    ,mSpotDuration{}
    ,mRealDuration{}
    ,mClient{}
    ,mBrand{}
{
    mName = createField<StringField>("name","Spot Name");
    mName->setMandatory(true);

    mSpotDuration = createField<DecimalField>("spot_duration", "Spot Duration");
    mRealDuration = createField<DecimalField>("real_duration", "Real Duration");
    mClient = createField<ForeignKeyField>("client_id", "Client",
                                           std::make_unique<Client>(), "name");
    mBrand = createField<ForeignKeyField>("brand_id", "Brand",
                                          std::make_unique<Brand>(),
                                          "brand_name");

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

    mHeader << QString::fromStdString(mName->fieldLabel())
             <<QString::fromStdString(mSpotDuration->fieldLabel());

    mVoiceOver = std::make_unique<VoiceOver>();
    mSpotVoiceOver = std::make_unique<SpotVoiceOver>(this, mVoiceOver.get());

    mTypeEx = std::make_unique<TypeExclusion>();
    mSpotTypeEx = std::make_unique<SpotTypeExclusion>(this, mTypeEx.get() );

    setTableName("rave_spot");
}

Spot::Spot(const Client* client)
    :mName{}
    ,mSpotDuration{}
    ,mRealDuration{}
    ,mClient{}
    ,mBrand{}
{
    mName = createField<StringField>("name","Spot Name");
    mName->setMandatory(true);

    mSpotDuration = createField<DecimalField>("spot_duration", "Spot Duration");
    mRealDuration = createField<DecimalField>("real_duration", "Real Duration");


   //auto filter = std::make_tuple("client_id", client.id());
    EntityDataModel edm;
    auto filter = std::make_tuple("client_id", client->id());
    std::string fstr = edm.prepareFilter(filter);


    mBrand = createField<ForeignKeyField>("brand_id", "Brand",
                                          std::make_unique<Brand>(),
                                          "brand_name",
                                          fstr);

    mClient = createField<ForeignKeyField>("client_id", "Client",
                                           std::make_unique<Client>(), "name");
    mClient->setValue(client->id());

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

    mHeader << QString::fromStdString(mName->fieldLabel())
             <<QString::fromStdString(mSpotDuration->fieldLabel());

    mVoiceOver = std::make_unique<VoiceOver>();
    mSpotVoiceOver = std::make_unique<SpotVoiceOver>(this, mVoiceOver.get());

    setTableName("rave_spot");
}

Spot::~Spot()
{
}

std::string Spot::tableName() const
{
    return mTableName;
}

void Spot::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> Spot::mapFields(StringMap*)
{
}

std::vector<std::string> Spot::tableViewColumns() const
{
    std::list<std::string> cols;

    return tableViewCols<std::string>(
                name()->displayName(),
                spotDuration()->displayName()
                );
}

std::vector<std::string> Spot::tableViewValues()
{
    return  tableViewColumns();
}

QStringList Spot::tableHeaders() const
{
    return mHeader;
}

std::string Spot::searchColumn() const
{
    return name()->valueToString();
}

void Spot::populateEntity()
{
}

std::unique_ptr<BaseEntity> Spot::cloneAsUnique()
{
    return std::make_unique<Spot>();
}

void Spot::afterMapping(BaseEntity &entity)
{
    mSpotVoiceOver->setParentId(entity.id());
    mSpotTypeEx->setParentId(entity.id());

}

StringField* Spot::name() const
{
    return mName;
}

void Spot::setName(const std::string n)
{
    mName->setValue(n);
}

DecimalField* Spot::spotDuration() const
{
    return mSpotDuration;
}

void Spot::setSpotDuration(double dur)
{
    mSpotDuration->setValue(dur);
}

DecimalField* Spot::realDuration() const
{
    return mRealDuration;
}

void Spot::setRealDuration(double dur)
{
    mRealDuration->setValue(dur);
}

ForeignKeyField* Spot::client() const
{
    return mClient;
}

void Spot::setClient(int client_id)
{
    mClient->setValue( client_id );
}

ForeignKeyField* Spot::brand() const
{
    return mBrand;
}

void Spot::setBrand(int brand_id)
{
    mBrand->setValue( brand_id );
}


StringField* Spot::daypart1() const
{
    return mDaypart1;
}
void Spot::setDaypart1(std::string dp)
{
    mDaypart1->setValue(dp);
}

StringField* Spot::daypart2() const
{
    return mDaypart2;
}
void Spot::setDaypart2(std::string dp)
{
    mDaypart2->setValue(dp);
}

StringField* Spot::daypart3() const
{
    return mDaypart3;
}
void Spot::setDaypart3(std::string dp)
{
    mDaypart3->setValue(dp);
}

StringField* Spot::daypart4() const
{
    return mDaypart4;
}
void Spot::setDaypart4(std::string dp)
{
    mDaypart4->setValue(dp);
}

StringField* Spot::daypart5() const
{
    return mDaypart5;
}
void Spot::setDaypart5(std::string dp)
{
    mDaypart5->setValue(dp);
}

StringField* Spot::daypart6() const
{
    return mDaypart6;
}
void Spot::setDaypart6(std::string dp)
{
    mDaypart6->setValue(dp);
}

StringField* Spot::daypart7() const
{
    return mDaypart7;
}
void Spot::setDaypart7(std::string dp)
{
    mDaypart7->setValue(dp);
}

SpotVoiceOver& Spot::spotVoiceOver()
{
    return *mSpotVoiceOver;
}

SpotTypeExclusion& Spot::spotTypeExclusion()
{
    return *mSpotTypeEx;
}
