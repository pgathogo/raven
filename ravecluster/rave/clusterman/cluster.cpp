#include "cluster.h"

namespace ClusterManager
{
    Cluster::Cluster()
    {
        m_cluster_name = createField<StringField>("cluster_name", "Cluster Name");
        m_cluster_name->setMandatory(true);

        m_notes = createField<TextField>("notes", "Notes");

        m_header << QString::fromStdString(m_cluster_name->fieldName());

        setTableName("rave_cluster");

    }

    Cluster::~Cluster()
    {
    }

    StringField* Cluster::cluster_name() const
    {
        return m_cluster_name;
    }

    TextField* Cluster::notes() const
    {
        return m_notes;
    }

    void Cluster::set_cluster_name(std::string name)
    {
        m_cluster_name->setValue(name);
    }

    void Cluster::set_notes(std::string nt)
    {
        m_notes->setValue(nt);
    }

    std::string Cluster::tableName() const
    {
        return "rave_cluster";
    }

    void Cluster::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }

    std::unique_ptr<BaseEntity> Cluster::mapFields(StringMap* smap)
    {
    }

    std::vector<std::string> Cluster::tableViewColumns() const
    {
        return tableViewCols<std::string>(cluster_name()->displayName());
    }

    std::vector<std::string> Cluster::tableViewValues()
    {
        return {cluster_name()->displayName()};
    }

    QStringList Cluster::tableHeaders() const
    {
        return m_header;
    }

    std::string Cluster::searchColumn() const
    {
        return cluster_name()->valueToString();
    }

    void Cluster::populateEntity()
    {
    }

    std::shared_ptr<BaseEntity> Cluster::cloneAsShared()
    {
        return std::make_shared<Cluster>();
    }

    void Cluster::afterMapping(BaseEntity& entity)
    {
    }

}
