#include "rolemember.h"
#include "user.h"

RoleMember::RoleMember()
    :ManyToMany{}
{
}

RoleMember::RoleMember(BaseEntity *pEnt, BaseEntity *dEnt)
    :ManyToMany{}
    ,mParentEntity{pEnt}
    ,mDetailEntity{dEnt}
{
    mParentId = createField<IntegerField>("roleid", "Role ID");
    mDetailId = createField<IntegerField>("member", "Member ID");

    mGrantor = createField<IntegerField>("grantor", "Grantor");
    mGrantor->setValue(10);
    mAdminOption = createField<BooleanField>("admin_option", "Admin Option");

    mHeader = mDetailEntity->tableHeaders();
    setTableName("pg_auth_members");
    getId().setFormOnly(true);
}

RoleMember::~RoleMember()
{

}

std::unique_ptr<ManyToMany> RoleMember::copy(BaseEntity *pEnt, BaseEntity *dEnt) const
{
    return std::make_unique<RoleMember>(pEnt, dEnt);

}

void RoleMember::setTable(const std::string tablename)
{
}

IntegerField *RoleMember::parentId() const
{
    return mParentId;
}

void RoleMember::setParentId(int id)
{
    mParentId->setValue( id );
}

IntegerField *RoleMember::detailId() const
{
    return mDetailId;
}

void RoleMember::setDetailId(int id)
{
    mDetailId->setValue( id );
}

IntegerField *RoleMember::grantor()
{
    return mGrantor;
}

void RoleMember::setGrantor(int val)
{
    mGrantor->setValue( val );
}

BooleanField *RoleMember::adminOption()
{
    return mAdminOption;
}

void RoleMember::setAdminOption(boolean val)
{
    mAdminOption->setValue( val );
}

BaseEntity *RoleMember::parentEntity() const
{
    return mParentEntity;
}

BaseEntity *RoleMember::detailEntity() const
{
    return mDetailEntity;
}

void RoleMember::setDetailEntity(BaseEntity *other)
{
    mDetailEntity = other;
}

std::unique_ptr<BaseEntity> RoleMember::mapFields(StringMap *sm)
{
}

std::vector<std::string> RoleMember::tableViewColumns() const
{
    User* user = dynamic_cast<User*>(mDetailEntity);

    return tableViewCols<std::string>(user->userName()->displayName());

}

std::vector<std::string> RoleMember::tableViewValues()
{
    User* user = dynamic_cast<User*>(mDetailEntity);

    std::string name = user->userName()->valueToString();

    return {name};
}

QStringList RoleMember::tableHeaders() const
{
    return mHeader;
}

std::string RoleMember::tableName() const
{
    return mTableName;
}

void RoleMember::setTableName(std::string table_name)
{
    mTableName = table_name;
}

std::string RoleMember::searchColumn() const
{
    return mDetailEntity->searchColumn();
}

void RoleMember::populateEntity()
{
    mParentId->setValue(mParentEntity->id());
    mDetailId->setValue(mDetailEntity->id());
}

BaseEntity* RoleMember::mtomEntity()
{
    return this;
}

std::unique_ptr<BaseEntity> RoleMember::cloneAsUnique()
{
    return std::make_unique<RoleMember>(mParentEntity, mDetailEntity);
}

void RoleMember::afterMapping(BaseEntity &entity)
{
    RoleMember& rm = dynamic_cast<RoleMember&>(entity);
    auto user = rm.detailEntity()->cloneAsUnique();
    rm.setDetailEntity( user.get() );

    /**********/
    getEntityById(std::move(user), rm.detailId()->value());
    /**********/

}


