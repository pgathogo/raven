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

        m_free_space = createField<DecimalField>("free_space", "Free Disk Space");
        m_raw_disk_name = createField<StringField>("raw_disk_name", "Raw Disk Name");
        m_last_refresh = createField<DateTimeField>("last_refresh", "Last Refresh Date");

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

    DecimalField* StorageDisk::free_space()
    {
        return m_free_space;
    }

    StringField* StorageDisk::raw_disk_name()
    {
        return m_raw_disk_name;
    }

    DateTimeField* StorageDisk::last_refresh()
    {
        return m_last_refresh;
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

    void StorageDisk::set_free_space(double fspace)
    {
        m_free_space->setValue(fspace);

    }

    void StorageDisk::set_raw_disk_name(const std::string dname)
    {
        m_raw_disk_name->setValue(dname);
    }

    void StorageDisk::set_last_refresh(QDateTime refdate)
    {
        m_last_refresh->setValue(refdate);
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

    std::shared_ptr<BaseEntity> StorageDisk::cloneAsShared()
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
