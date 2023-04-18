#include "orderapprover.h"
#include "../security/user.h"

OrderApprover::OrderApprover()
{

    mUserId = createField<ForeignKeyField>("user_id", "User ID",
                                           std::make_unique<User>(), "usename");
    mUserName = createField<StringField>("username", "User Name");
    mUserTitle = createField<StringField>("user_title", "User Title");
    mLevel = createField<IntegerField>("level", "Level");

    mHeader << stoq(mUserName->fieldLabel())
            << stoq(mUserTitle->fieldLabel())
            << stoq(mLevel->fieldLabel());

    setTableName("rave_orderapprover");
}

OrderApprover::~OrderApprover()
{
}

ForeignKeyField *OrderApprover::userId()
{
    return mUserId;
}

void OrderApprover::setUserId(int id)
{
    mUserId->setValue( id );
}

StringField *OrderApprover::userName() const
{
    return mUserName;
}

void OrderApprover::setUserName(const std::string username)
{
    mUserName->setValue( username );
}

StringField *OrderApprover::userTitle() const
{
    return mUserTitle;
}

void OrderApprover::setUserTitle(const std::string title)
{
    mUserTitle->setValue( title );
}

IntegerField *OrderApprover::level() const
{
    return mLevel;
}

void OrderApprover::setLevel(int lvl)
{
    mLevel->setValue( lvl );
}

std::string OrderApprover::tableName() const
{
   return mTableName;
}

void OrderApprover::setTableName(const std::string table_name)
{
    mTableName = table_name;

}

std::unique_ptr<BaseEntity> OrderApprover::mapFields(StringMap*)
{
    return nullptr;
}

std::vector<std::string> OrderApprover::tableViewColumns() const
{
    return tableViewCols<std::string>(
                userName()->displayName(),
                userTitle()->displayName(),
                level()->displayName()
                );
}

std::vector<std::string> OrderApprover::tableViewValues()
{
    return  tableViewColumns();
}

QStringList OrderApprover::tableHeaders() const
{
    return mHeader;
}

std::string OrderApprover::searchColumn() const
{
    return userName()->valueToString();
}

void OrderApprover::populateEntity()
{
}

std::shared_ptr<BaseEntity> OrderApprover::cloneAsShared()
{
    return std::make_shared<OrderApprover>();
}

void OrderApprover::afterMapping(BaseEntity&)
{
}
