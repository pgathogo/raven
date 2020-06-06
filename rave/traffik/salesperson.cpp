#include "salesperson.h"

#include "../framework/valuelist.h"
#include "../framework/entitydatamodel.h"

EntityDataModel* SalesPerson::mGenderDM{};

SalesPerson::SalesPerson()
    :BaseEntity{}
    ,mName{}
    ,mMobileNo{}
    ,mGender{}
{
    if (mGenderDM == nullptr)
        mGenderDM = new EntityDataModel(new Gender());

    mName = createField<StringField>("salesperson_name", "Sales Person Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);

    mMobileNo = createField<StringField>("mobile_no", "Mobile No.");

    mGender = createField<LookupField>("gender", "Gender", mGenderDM);
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

QList<QStandardItem*> SalesPerson::tableViewColumns()
{
    QString nm = QString::fromStdString(name()->displayName());
    QString gen = QString::fromStdString(gender()->displayName());

    QStandardItem* qname = new QStandardItem(nm);
    QStandardItem* qgen = new QStandardItem(gen);

    return {qname, qgen};
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
   // mName->setValueFromWidget();
   // mMobileNo->setValueFromWidget();
   // mGender->setValueFromWidget();
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
