#include "salesperson.h"

#include "../framework/valuelist.h"
#include "../framework/entitydatamodel.h"

SalesPerson::SalesPerson()
    :BaseEntity{}
    ,mName{}
    ,mMobileNo{}
    ,mGender{}
{

    mName = createField<StringField>("salesperson_name", "Sales Person Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);

    mMobileNo = createField<StringField>("mobile_no", "Mobile No.");

    mGender = createField<ForeignKeyField>("gender", "Gender",
                                           std::make_unique<Gender>(), "value");
    mGender->setDBColumnName("gender_id");

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mGender->fieldLabel());

    setTableName("rave_salesperson");
}

SalesPerson::~SalesPerson()
{
}

std::string SalesPerson::tableName() const
{
    return mTableName;
}

void SalesPerson::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> SalesPerson::mapFields(StringMap* raw_entity)
{
    std::unique_ptr<SalesPerson> sp = entityFieldMap<SalesPerson>(raw_entity);
    return std::move(sp);
}

std::vector<std::string> SalesPerson::tableViewColumns() const
{
    return tableViewCols<std::string>(
                name()->displayName(),
                name()->displayName()
                );
}

std::vector<std::string> SalesPerson::tableViewValues()
{
    return tableViewColumns();
}

QStringList SalesPerson::tableHeaders() const
{
    return mHeader;
}

std::string SalesPerson::searchColumn() const
{
    return name()->valueToString();
}

void SalesPerson::populateEntity()
{
}

StringField* SalesPerson::name() const
{
    return mName;
}

void SalesPerson::setName(std::string aName)
{
    mName->setValue(aName);
}

StringField* SalesPerson::mobileno() const
{
    return mMobileNo;
}

void SalesPerson::setMobileNo(std::string mobileno)
{
    mMobileNo->setValue(mobileno);
}

ForeignKeyField* SalesPerson::gender() const
{
    return mGender;
}

void SalesPerson::setGender(int index)
{
    mGender->setIndex(index);
}


std::unique_ptr<BaseEntity> SalesPerson::cloneAsUnique()
{
    return std::make_unique<SalesPerson>();
}

void SalesPerson::afterMapping(BaseEntity &entity)
{

}
