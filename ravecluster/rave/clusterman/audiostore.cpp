#include "audiostore.h"

#include "server.h"
#include "station.h"

namespace ClusterManager
{

    AudioStore::AudioStore()
    {
      m_store_name = createField<StringField>("store_name", "Store Name");
      m_notes = createField<TextField>("notes", "Notes");

      m_server = createField<ForeignKeyField>("server", "Server",
                                              std::make_unique<Server>(), "server_name");

      m_station = createField<ForeignKeyField>("station", "Station",
                                               std::make_unique<Station>(), "station_name");

      m_header << QString::fromStdString(m_store_name->fieldName())
               << QString::fromStdString(m_server->fieldName());

      setTableName("rave_audiostore");
    }

    AudioStore::~AudioStore()
    {

    }

    StringField* AudioStore::store_name() const
    {
        return m_store_name;
    }

    ForeignKeyField* AudioStore::server() const
    {
        return m_server;
    }

    ForeignKeyField* AudioStore::station() const
    {
        return m_station;
    }

    TextField* AudioStore::notes() const
    {
        return m_notes;
    }

    void AudioStore::set_store_name(const std::string name)
    {
        m_store_name->setValue(name);
    }

    void AudioStore::set_server(int server_id)
    {
        m_server->setValue(server_id);
    }

    void AudioStore::set_station(int sid)
    {
        m_station->setValue(sid);

    }

    void AudioStore::set_notes(const std::string nts)
    {
        m_notes->setValue(nts);
    }

    std::string AudioStore::tableName() const
    {
        return "rave_audiostore";
    }

    std::unique_ptr<BaseEntity> AudioStore::mapFields(StringMap*)
    {
    }

    std::vector<std::string> AudioStore::tableViewColumns() const
    {
        return tableViewCols<std::string>(store_name()->displayName());
    }

    std::vector<std::string> AudioStore::tableViewValues()
    {
        return {store_name()->displayName()};
    }

    QStringList AudioStore::tableHeaders() const
    {
        return m_header;

    }
    std::string AudioStore::searchColumn() const
    {
        return store_name()->valueToString();
    }

    std::shared_ptr<BaseEntity> AudioStore::cloneAsShared()
    {
        return std::make_shared<Server>();
    }

    void AudioStore::populateEntity()
    {
    }

    void AudioStore::afterMapping(BaseEntity&)
    {
    }

    void AudioStore::setTableName(const std::string table)
    {
        m_table_name = table;
    }


}
