#include "spottypeexclusion.h"

#include "typeexclusion.h"

SpotTypeExclusion::SpotTypeExclusion()
    :ManyToMany{}
{
}

SpotTypeExclusion::SpotTypeExclusion(BaseEntity *pEnt, BaseEntity *dEnt)
    :ManyToMany{}
    ,mParentEntity{pEnt}
    ,mDetailEntity{dEnt}
{

    mParentId = createField<IntegerField>("parent_id","Parent ID");
    mDetailId = createField<IntegerField>("detail_id","Detail ID");

    mHeader = mDetailEntity->tableHeaders();
    setTableName("rave_spottypeexclusion");
}

SpotTypeExclusion::~SpotTypeExclusion()
{

}

std::unique_ptr<ManyToMany> SpotTypeExclusion::copy(BaseEntity *pEnt, BaseEntity *dEnt) const
{
    return std::make_unique<SpotTypeExclusion>(pEnt, dEnt);
}

IntegerField *SpotTypeExclusion::parentId() const
{
    return mParentId;
}

void SpotTypeExclusion::setParentId(int id)
{
    mParentId->setValue( id );
}

IntegerField *SpotTypeExclusion::detailId() const
{
    return mDetailId;
}

void SpotTypeExclusion::setDetailId(int id)
{
    mDetailId->setValue( id );
}

BaseEntity *SpotTypeExclusion::parentEntity() const
{
    return mParentEntity;
}

BaseEntity *SpotTypeExclusion::detailEntity() const
{
    return mDetailEntity;
}

void SpotTypeExclusion::setDetailEntity(BaseEntity *other)
{
    mDetailEntity = other;
}

std::unique_ptr<BaseEntity> SpotTypeExclusion::mapFields(StringMap*)
{
}

std::vector<std::string> SpotTypeExclusion::tableViewColumns() const
{
    TRAFFIK::TypeExclusion* te = dynamic_cast<TRAFFIK::TypeExclusion*>(mDetailEntity);

    return tableViewCols<std::string>(
                te->name()->displayName(),
                te->description()->displayName()
                );
}

std::vector<std::string> SpotTypeExclusion::tableViewValues()
{
    TRAFFIK::TypeExclusion* te = dynamic_cast<TRAFFIK::TypeExclusion*>(mDetailEntity);

    return tableViewCols<std::string>(
                te->name()->valueToString(),
                te->description()->valueToString()
                );
}

QStringList SpotTypeExclusion::tableHeaders() const
{
    return mHeader;
}

std::string SpotTypeExclusion::tableName() const
{
    return mTableName;
}

void SpotTypeExclusion::setTableName(std::string table_name)
{
    mTableName = table_name;
}

std::string SpotTypeExclusion::searchColumn() const
{
    return mDetailEntity->searchColumn();
}

void SpotTypeExclusion::populateEntity()
{
    mParentId->setValue(mParentEntity->id());
    mDetailId->setValue(mDetailEntity->id());
}

BaseEntity *SpotTypeExclusion::mtomEntity()
{
    return this;
}

std::unique_ptr<BaseEntity> SpotTypeExclusion::cloneAsUnique()
{
    return std::make_unique<SpotTypeExclusion>(mParentEntity, mDetailEntity);
}

void SpotTypeExclusion::afterMapping(BaseEntity &entity)
{
    SpotTypeExclusion& svo = dynamic_cast<SpotTypeExclusion&>(entity);
    auto vo = svo.detailEntity()->cloneAsUnique();
    svo.setDetailEntity( vo.get() );
    getEntityById(std::move(vo), svo.detailId()->value());
}
