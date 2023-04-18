#include "brand.h"
#include "client.h"
#include "salesperson.h"

namespace TRAFFIK {

    Brand::Brand()
        :m_brand_name{nullptr}
        ,m_client{nullptr}
        ,m_brand_manager{nullptr}
    {
        m_brand_name = createField<StringField>("brand_name", "Brand Name");
        m_brand_name->setMandatory(true);
        m_client = createField<ForeignKeyField>("client_id", "Client",
                                               std::make_unique<Client>(), "name");
        m_client->setMandatory(true);
        m_brand_manager = createField<ForeignKeyField>("brand_manager_id", "Brand Manager",
                                                     std::make_unique<SalesPerson>(), "salesperson_name");

        mHeader << QString::fromStdString(m_brand_name->fieldLabel());

        setTableName("rave_brand");
    }

    Brand::Brand(const Client* client)
        :m_brand_name{nullptr}
        ,m_client{}
        ,m_brand_manager{nullptr}
    {
        m_brand_name = createField<StringField>("brand_name", "Brand Name");
        m_brand_name->setMandatory(true);
        m_client = createField<ForeignKeyField>("client_id", "Client",
                                               std::make_unique<Client>(), "name");
        m_client->setValue(client->id());
        m_client->setMandatory(true);

        m_brand_manager = createField<ForeignKeyField>("brand_manager_id", "Brand Manager",
                                                     std::make_unique<SalesPerson>(), "salesperson_name");

        mHeader << QString::fromStdString(m_brand_name->fieldLabel());

        setTableName("rave_brand");
    }



    Brand::~Brand()
    {
    }

    void Brand::set_brand_name(std::string brand_name)
    {
        m_brand_name->setValue(brand_name);
    }

    StringField* Brand::brand_name() const
    {
        return m_brand_name;
    }

    void Brand::set_client(int client_id)
    {
        m_client->setValue(client_id);
    }

    ForeignKeyField* Brand::client() const
    {
        return m_client;
    }

    void Brand::set_brand_manager(int brand_manager_id)
    {
        m_brand_manager->setValue(brand_manager_id);
    }

    ForeignKeyField* Brand::brand_manager() const
    {
        return m_brand_manager;
    }

    std::string Brand::tableName() const
    {
        return "rave_brand";
    }

    void Brand::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::shared_ptr<BaseEntity> Brand::cloneAsShared()
    {
        return std::make_shared<Brand>();
    }

    void Brand::afterMapping(BaseEntity &entity)
    {

    }

    std::unique_ptr<BaseEntity> Brand::mapFields(StringMap* e)
    {
        std::unique_ptr<Brand> brand = entityFieldMap<Brand>(e);
        return std::move(brand);
    }

    std::vector<std::string> Brand::tableViewColumns() const
    {
        return tableViewCols<std::string>(brand_name()->displayName());
    }

    std::vector<std::string> Brand::tableViewValues()
    {
        return{brand_name()->displayName()};
    }

    QStringList Brand::tableHeaders() const
    {
        return mHeader;
    }

    std::string Brand::searchColumn() const
    {
        return brand_name()->valueToString();
    }


    void Brand::populateEntity()
    {
    }

}
