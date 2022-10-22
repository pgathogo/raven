#include "storagedisk.h"

#include "server.h"

namespace ClusterManager
{

    StorageDisk::StorageDisk()
    {
        m_disk_name = createField<StringField>("disk_name", "Disk Name");
        m_capacity = createField<DecimalField>("capacity", "Capacity");
        m_audio_server = createField<ForeignKeyField>("audio_server_id", "Audio Server",
                                                     std::make_unique<Server>(), "server_name");

        m_header << QString::fromStdString(m_disk_name->fieldName())
                 << QString::fromStdString(m_audio_server->fieldName());

        setTableName("rave_storagedisk");

    }

    StorageDisk::~StorageDisk()
    {

    }

    StringField* StorageDisk::disk_name() const
    {
        return m_disk_name;
    }

    ForeignKeyField* StorageDisk::audio_server()
    {
        return m_audio_server;
    }

    DecimalField* StorageDisk::capacity()
    {
        return m_capacity;
    }

    void StorageDisk::set_disk_name(const std::string name)
    {
       m_disk_name->setValue(name);
    }

    void StorageDisk::set_audio_server(int server)
    {
        m_audio_server->setValue(server);
    }
    void StorageDisk::set_capacity(double cap)
    {
        m_capacity->setValue(cap);
    }

    std::string StorageDisk::tableName() const
    {
        return m_table_name;
    }

    std::unique_ptr<BaseEntity> StorageDisk::mapFields(StringMap*)
    {

    }

    std::vector<std::string> StorageDisk::tableViewColumns() const
    {
       return tableViewCols<std::string>(disk_name()->displayName()) ;
    }

    std::vector<std::string> StorageDisk::tableViewValues()
    {
        return {disk_name()->displayName()};
    }

    QStringList StorageDisk::tableHeaders() const
    {
        return m_header;
    }

    std::string StorageDisk::searchColumn() const
    {
        return disk_name()->valueToString();
    }

    std::unique_ptr<BaseEntity> StorageDisk::cloneAsUnique()
    {
    }

    void StorageDisk::populateEntity()
    {

    }

    void StorageDisk::afterMapping(BaseEntity&)
    {

    }

    void StorageDisk::setTableName(const std::string table)
    {
       m_table_name = table;
    }
}
