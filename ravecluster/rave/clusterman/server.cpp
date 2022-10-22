#include "server.h"

#include "../../../framework/choicefield.h"

#include "cluster.h"

namespace ClusterManager
{
    Server::Server()
    {
         m_server_name = createField<StringField>("server_name", "Server Name");
         m_server_name->setMandatory(true);

         m_server_ip = createField<StringField>("server_ip", "IP Address");

         m_server_type = createField<ChoiceField<std::string>>("server_type", "Server Type");
         m_server_type->addChoice({"DBS", "DB-Server"});
         m_server_type->addChoice({"ADS", "Audio-Server"});
         m_server_type->setMandatory(true);

         m_cluster = createField<ForeignKeyField>("cluster_id", "Cluster",
                                                  std::make_unique<Cluster>(), "cluster_name");

        m_header << QString::fromStdString(m_server_name->fieldLabel())
                 << QString::fromStdString(m_server_ip->fieldLabel());

        setTableName("rave_server");

    }

    Server::~Server()
    {
    }

    StringField* Server::server_name() const
    {
        return m_server_name;
    }

    StringField* Server::server_ip() const
    {
        return m_server_ip;
    }

    ChoiceField<std::string>* Server::server_type() const
    {
        return m_server_type;
    }

    ForeignKeyField* Server::cluster()
    {
        return m_cluster;
    }

    void Server::set_server_name(const std::string name)
    {
        m_server_name->setValue(name);
    }

    void Server::set_server_ip(const std::string ip)
    {
        m_server_ip->setValue(ip);
    }

    void Server::set_server_type(const std::string type)
    {
        m_server_type->setValue(type);
    }

    void Server::set_cluster(int cluster_id)
    {
        m_cluster->setValue(cluster_id);
    }

    std::string Server::tableName() const
    {
        return "rave_server";
    }

    std::unique_ptr<BaseEntity> Server::mapFields(StringMap*)
    {
    }

    std::vector<std::string> Server::tableViewColumns() const
    {
        return tableViewCols<std::string>(server_name()->displayName());
    }

    std::vector<std::string> Server::tableViewValues()
    {
        return {server_name()->displayName()};
    }

    QStringList Server::tableHeaders() const
    {
        return m_header;

    }
    std::string Server::searchColumn() const
    {
        return server_name()->valueToString();
    }

    std::unique_ptr<BaseEntity> Server::cloneAsUnique()
    {
        return std::make_unique<Server>();
    }

    void Server::populateEntity()
    {
    }

    void Server::afterMapping(BaseEntity&)
    {
    }

    void Server::setTableName(const std::string table)
    {
        m_table_name = table;
    }


}
