#include "database.h"
#include "server.h"

namespace ClusterManager
{

    Database::Database()
    {
        m_db_name = createField<StringField>("db_name", "Database Name");
        m_db_port = createField<IntegerField>("db_port", "DB Port");
        m_server = createField<ForeignKeyField>("server", "DB Server",
                                                std::make_unique<Server>(), "server_name");

        m_header << QString::fromStdString(m_db_name->fieldLabel())
                 << QString::fromStdString(m_db_port->fieldLabel())
                 << QString::fromStdString(m_server->fieldLabel());

        setTableName("rave_database");
    }

    Database::~Database()
    {
    }

    StringField* Database::db_name() const
    {
        return m_db_name;
    }

    ForeignKeyField* Database::server() const
    {
        return m_server;
    }

    IntegerField* Database::db_port() const
    {
        return m_db_port;
    }

    void Database::set_db_name(const std::string name)
    {
        m_db_name->setValue(name);

    }

    void Database::set_server(int server_id)
    {
        m_server->setValue(server_id);
    }

    void Database::set_db_port(int port)
    {
        m_db_port->setValue(port);
    }

    std::string Database::tableName() const
    {
        return "rave_server";
    }

    std::unique_ptr<BaseEntity> Database::mapFields(StringMap*)
    {
    }

    std::vector<std::string> Database::tableViewColumns() const
    {
        return tableViewCols<std::string>(db_name()->displayName());
    }

    std::vector<std::string> Database::tableViewValues()
    {
        return {db_name()->displayName()};
    }

    QStringList Database::tableHeaders() const
    {
        return m_header;

    }
    std::string Database::searchColumn() const
    {
        return db_name()->valueToString();
    }

    std::unique_ptr<BaseEntity> Database::cloneAsUnique()
    {
        return std::make_unique<Server>();
    }

    void Database::populateEntity()
    {
    }

    void Database::afterMapping(BaseEntity&)
    {
    }

    void Database::setTableName(const std::string table)
    {
        m_table_name = table;
    }

}
