#include "salesperson.h"

#include "../framework/valuelist.h"
#include "../framework/entitydatamodel.h"

//EntityDataModel* SalesPerson::mGenderDM{nullptr};

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

    mGender = createField<LookupField>("gender", "Gender", new Gender());
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

std::list<std::string> SalesPerson::tableViewColumns()
{
    std::list<std::string> cols;

    cols.push_back(name()->displayName());
    cols.push_back(name()->displayName());

    return cols;

    /*
    QString nm = QString::fromStdString(name()->displayName());
    QString gen = QString::fromStdString(gender()->displayName());

    QStandardItem* qname = new QStandardItem(nm);
    QStandardItem* qgen = new QStandardItem(gen);

    return {qname, qgen};
    */
}

std::vector<std::string> SalesPerson::tableViewValues()
{
    return {name()->displayName(), gender()->displayName()};
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

LookupField* SalesPerson::gender() const
{
    return mGender;
}

void SalesPerson::setGender(int index)
{
    mGender->setIndex(index);
}
