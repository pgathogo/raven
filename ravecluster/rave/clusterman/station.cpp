#include "station.h"
#include "cluster.h"

namespace ClusterManager
{
    Station::Station()
    {
        m_station_name = createField<StringField>("station_name", "Station Name");
        m_station_name->setMandatory(true);

        m_cluster = createField<ForeignKeyField>("cluster_id", "Cluster",
                                                 std::make_unique<Cluster>(), "cluster_name");

        m_header << QString::fromStdString(station_name()->fieldName())
                 << QString::fromStdString(cluster()->fieldName());


        setTableName("rave_station");
    };

    Station::~Station()
    {
    }

    StringField* Station::station_name() const
    {
        return m_station_name;
    }

    ForeignKeyField* Station::cluster() const
    {
        return m_cluster;
    }

    void Station::set_station_name(const std::string name)
    {
        m_station_name->setValue(name);
    }

    void Station::set_cluster(int cluster_id)
    {
        m_cluster->setValue(cluster_id);
    }

    std::string Station::tableName() const
    {
        return "rave_station";
    }

    std::unique_ptr<BaseEntity> Station::mapFields(StringMap*)
    {
    }

    std::vector<std::string> Station::tableViewColumns() const
    {
        return tableViewCols<std::string>(station_name()->displayName());
    }

    std::vector<std::string> Station::tableViewValues()
    {
        return {station_name()->displayName()};
    }

    QStringList Station::tableHeaders() const
    {
        return m_header;

    }
    std::string Station::searchColumn() const
    {
        return station_name()->valueToString();
    }

    std::shared_ptr<BaseEntity> Station::cloneAsShared()
    {
        return std::make_shared<Station>();
    }

    void Station::populateEntity()
    {
    }

    void Station::afterMapping(BaseEntity&)
    {
    }

    void Station::setTableName(const std::string table)
    {
        m_table_name = table;
    }

}
