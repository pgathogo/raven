#include "spotvoiceover.h"
#include "voiceover.h"

SpotVoiceOver::SpotVoiceOver()
    :ManyToMany{}
{
}

SpotVoiceOver::SpotVoiceOver(BaseEntity *pEnt, BaseEntity *dEnt)
    :ManyToMany{}
    ,mParentEntity{pEnt}
    ,mDetailEntity{dEnt}
{

    mParentId = createField<IntegerField>("parent_id","Parent ID");
    mDetailId = createField<IntegerField>("detail_id","Detail ID");

    mHeader = mDetailEntity->tableHeaders();
}

SpotVoiceOver::~SpotVoiceOver()
{

}

std::shared_ptr<ManyToMany> SpotVoiceOver::copy(BaseEntity *pEnt, BaseEntity *dEnt) const
{
    return std::make_shared<SpotVoiceOver>(pEnt, dEnt);
}

IntegerField *SpotVoiceOver::parentId() const
{
    return mParentId;
}

void SpotVoiceOver::setParentId(int id)
{
    mParentId->setValue( id );
}

IntegerField *SpotVoiceOver::detailId() const
{
    return mDetailId;
}

void SpotVoiceOver::setDetailId(int id)
{
    mDetailId->setValue( id );
}

BaseEntity *SpotVoiceOver::parentEntity() const
{
    return mParentEntity;
}

BaseEntity *SpotVoiceOver::detailEntity() const
{
    return mDetailEntity;
}

void SpotVoiceOver::setDetailEntity(BaseEntity *other)
{
    mDetailEntity = other;
}

std::unique_ptr<BaseEntity> SpotVoiceOver::mapFields(StringMap*)
{
    return nullptr;
}

std::vector<std::string> SpotVoiceOver::tableViewColumns() const
{
    VoiceOver* vo = dynamic_cast<VoiceOver*>(mDetailEntity);

    return tableViewCols<std::string>(
                vo->name()->displayName(),
                vo->gender()->displayName()
                );
}

std::vector<std::string> SpotVoiceOver::tableViewValues()
{
    VoiceOver* vo = dynamic_cast<VoiceOver*>(mDetailEntity);

    return tableViewCols<std::string>(
                vo->name()->valueToString(),
                vo->gender()->valueToString()
                );
}

QStringList SpotVoiceOver::tableHeaders() const
{
    return mHeader;
}

std::string SpotVoiceOver::tableName() const
{
    return "rave_spotvoiceover";

}

void SpotVoiceOver::setTableName(std::string table_name)
{
    mTableName = table_name;
}

std::string SpotVoiceOver::searchColumn() const
{
    return mDetailEntity->searchColumn();
}

void SpotVoiceOver::populateEntity()
{
    mParentId->setValue(mParentEntity->id());
    mDetailId->setValue(mDetailEntity->id());
}

BaseEntity *SpotVoiceOver::mtomEntity()
{
    return this;
}

std::shared_ptr<BaseEntity> SpotVoiceOver::cloneAsShared()
{
    return std::make_shared<SpotVoiceOver>(mParentEntity, mDetailEntity);
}

void SpotVoiceOver::afterMapping(BaseEntity &entity)
{
    SpotVoiceOver& svo = dynamic_cast<SpotVoiceOver&>(entity);
    auto vo = svo.detailEntity()->cloneAsShared();
    svo.setDetailEntity( vo.get() );
    getEntityById(std::move(vo), svo.detailId()->value());
}
