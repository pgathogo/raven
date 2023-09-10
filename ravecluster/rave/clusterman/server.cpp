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
         m_port_no = createField<IntegerField>("port_no", "Port Number");

         m_server_type = createField<ChoiceField<std::string>>("server_type", "Server Type");
         m_server_type->addChoice({"DBS", "DB-Server"});
         m_server_type->addChoice({"ADS", "Audio-Server"});
         m_server_type->setMandatory(true);

         m_cluster = createField<ForeignKeyField>("cluster_id", "Cluster",
                                                  std::make_unique<Cluster>(), "cluster_name");

         m_db_admin =  createField<StringField>("db_admin", "DB Admin");
         m_db_admin_password = createField<StringField>("db_admin_password", "Admin Password");

        m_header << QString::fromStdString(m_server_name->fieldLabel())
                 << QString::fromStdString(m_server_ip->fieldLabel());

        setTableName("rave_server");

    }

    Server& Server::operator=(const Server& other)
    {
        if (this == &other) return *this;

        this->setId(other.id());
        m_server_name->setValue(other.server_name()->value());
        m_server_ip->setValue(other.server_ip()->value());
        m_server_type->setValue(other.server_type()->value());
        m_port_no->setValue(other.port_no()->value());

        m_cluster->setValue(other.cluster()->value());

        m_db_admin->setValue(other.db_admin()->value());
        m_db_admin_password->setValue(other.db_admin_password()->value());

        setTableName("rave_server");
        return *this;
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

    IntegerField* Server::port_no() const
    {
        return m_port_no;
    }

    ChoiceField<std::string>* Server::server_type() const
    {
        return m_server_type;
    }

    ForeignKeyField* Server::cluster() const
    {
        return m_cluster;
    }

    StringField* Server::db_admin() const
    {
        return m_db_admin;

    }

    StringField* Server::db_admin_password() const
    {
        return m_db_admin_password;
    }

    void Server::set_server_name(const std::string name)
    {
        m_server_name->setValue(name);
    }

    void Server::set_server_ip(const std::string ip)
    {
        m_server_ip->setValue(ip);
    }

    void Server::set_port_no(int port)
    {
        m_port_no->setValue(port);
    }

    void Server::set_server_type(const std::string type)
    {
        m_server_type->setValue(type);
    }

    void Server::set_cluster(int cluster_id)
    {
        m_cluster->setValue(cluster_id);
    }

    void Server::set_db_admin(const std::string name)
    {
        m_db_admin->setValue(name);

    }

    void Server::set_db_admin_password(const std::string password)
    {
       m_db_admin_password->setValue(password);

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

    std::shared_ptr<BaseEntity> Server::cloneAsShared()
    {
        return std::make_shared<Server>();
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
