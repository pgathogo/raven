#include "orderpackage.h"

OrderPackage::OrderPackage()
{
    mName = createField<StringField>("name", "Package Name");
    mSpotCount = createField<IntegerField>("spot_count", "Spot Count");
    mMentionCount = createField<IntegerField>("mention_count", "Mention Count");

    mHeader << QString::fromStdString(mName->fieldLabel());
    setTableName("rave_orderpackage");
}

OrderPackage::~OrderPackage()
{

}

StringField *OrderPackage::name() const
{
    return mName;
}

void OrderPackage::setName(const std::string val)
{
    mName->setValue(val);
}

IntegerField *OrderPackage::spotCount() const
{
    return mSpotCount;
}

void OrderPackage::setSpotCount(int i)
{
    mSpotCount->setValue(i);
}

IntegerField *OrderPackage::mentionCount() const
{
    return mMentionCount;
}

void OrderPackage::setMentionCount(int i)
{
    mMentionCount->setValue(i);
}

std::string OrderPackage::tableName() const
{
    return mTableName;
}

void OrderPackage::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> OrderPackage::mapFields(StringMap*)
{
    return nullptr;
}

std::vector<std::string> OrderPackage::tableViewColumns() const
{
    return tableViewCols<std::string>(name()->displayName());
}

std::vector<std::string> OrderPackage::tableViewValues()
{
    return {name()->displayName()};
}

QStringList OrderPackage::tableHeaders() const
{
    return mHeader;
}

std::string OrderPackage::searchColumn() const
{
    return name()->valueToString();
}

void OrderPackage::populateEntity()
{

}

std::shared_ptr<BaseEntity> OrderPackage::cloneAsShared()
{
    return std::make_shared<OrderPackage>();
}

void OrderPackage::afterMapping(BaseEntity &entity)
{
}
