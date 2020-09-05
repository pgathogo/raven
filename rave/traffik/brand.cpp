#include "brand.h"
#include "client.h"
#include "salesperson.h"

Brand::Brand()
    :mBrandName{nullptr}
    ,mClient{nullptr}
    ,mBrandManager{nullptr}
{
    mBrandName = createField<StringField>("brand_name", "Brand Name");
    mBrandName->setMandatory(true);
    mClient = createField<ForeignKeyField>("client_id", "Client",
                                           std::make_unique<Client>(), "name");
    mClient->setMandatory(true);
    mBrandManager = createField<ForeignKeyField>("brand_manager_id", "Brand Manager",
                                                 std::make_unique<SalesPerson>(), "salesperson_name");

    mHeader << QString::fromStdString(mBrandName->fieldLabel());

    setTableName("rave_brand");
}

Brand::Brand(const Client* client)
    :mBrandName{nullptr}
    ,mClient{}
    ,mBrandManager{nullptr}
{
    mBrandName = createField<StringField>("brand_name", "Brand Name");
    mBrandName->setMandatory(true);
    mClient = createField<ForeignKeyField>("client_id", "Client",
                                           std::make_unique<Client>(), "name");
    mClient->setValue(client->id());
    mClient->setMandatory(true);

    mBrandManager = createField<ForeignKeyField>("brand_manager_id", "Brand Manager",
                                                 std::make_unique<SalesPerson>(), "salesperson_name");

    mHeader << QString::fromStdString(mBrandName->fieldLabel());

    setTableName("rave_brand");
}



Brand::~Brand()
{
}

void Brand::setBrandName(std::string brand_name)
{
    mBrandName->setValue(brand_name);
}

StringField* Brand::brandName() const
{
    return mBrandName;
}

void Brand::setClient(int client_id)
{
    mClient->setValue(client_id);
}

ForeignKeyField* Brand::client() const
{
    return mClient;
}

void Brand::setBrandManager(int brand_manager_id)
{
    mBrandManager->setValue(brand_manager_id);
}

ForeignKeyField* Brand::brandManager() const
{
    return mBrandManager;
}

std::string Brand::tableName() const
{
    return "rave_brand";
}

void Brand::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> Brand::cloneAsUnique()
{
    return std::make_unique<Brand>();
}

void Brand::afterMapping(BaseEntity &entity)
{

}

std::unique_ptr<BaseEntity> Brand::mapFields(StringMap* e)
{
    std::unique_ptr<Brand> brand = entityFieldMap<Brand>(e);
    return std::move(brand);
}

std::list<std::string> Brand::tableViewColumns() const
{
    std::list<std::string> cols;
    cols.push_back(brandName()->displayName());

    return cols;
}

std::vector<std::string> Brand::tableViewValues()
{
    std::string bname = brandName()->displayName();
    return {bname};
}

QStringList Brand::tableHeaders() const
{
    return mHeader;
}

std::string Brand::searchColumn() const
{
    return brandName()->valueToString();
}


void Brand::populateEntity()
{
}
