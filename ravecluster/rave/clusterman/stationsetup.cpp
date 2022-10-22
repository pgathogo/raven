#include "stationsetup.h"

#include "station.h"
#include "audiostore.h"
#include "audiofolder.h"
#include "database.h"

namespace ClusterManager
{

    StationSetup::StationSetup()
    {
        m_station =  createField<ForeignKeyField>("station", "Station",
                                                  std::make_unique<Station>(), "station_name");
        m_audio_store = createField<ForeignKeyField>("audio_store", "Audio Store",
                                                   std::make_unique<AudioStore>(), "store_name");
        m_audio_folder = createField<ForeignKeyField>("audio_folder", "Audio Folder",
                                                      std::make_unique<AudioFolder>(), "audio_folder");
        m_database = createField<ForeignKeyField>("database", "Database",
                                                  std::make_unique<Database>(), "db_name") ;

        m_header << QString::fromStdString(m_station->fieldName());

        setTableName("rave_stationsetup");
    }

    StationSetup::~StationSetup()
    {
    }

    ForeignKeyField* StationSetup::station() const
    {
        return m_station;
    }
    ForeignKeyField* StationSetup::audio_store() const
    {
        return m_audio_store;
    }

    ForeignKeyField* StationSetup::audio_folder() const
    {
        return m_audio_folder;
    }

    ForeignKeyField* StationSetup::database() const
    {
        return m_database;
    }

    void StationSetup::set_station(int s_id)
    {
        m_station->setValue(s_id);
    }

    void StationSetup::set_audio_store(int a_id)
    {
        m_audio_store->setValue(a_id);
    }

    void StationSetup::set_audio_folder(int af_id)
    {
        m_audio_folder->setValue(af_id);
    }

    void StationSetup::set_database(int db_id)
    {
        m_database->setValue(db_id);
    }

    std::string StationSetup::tableName() const
    {
        return "rave_station";
    }

    std::unique_ptr<BaseEntity> StationSetup::mapFields(StringMap*)
    {
    }

    std::vector<std::string> StationSetup::tableViewColumns() const
    {
        return tableViewCols<std::string>(station()->displayName());
    }

    std::vector<std::string> StationSetup::tableViewValues()
    {
        return {station()->displayName()};
    }

    QStringList StationSetup::tableHeaders() const
    {
        return m_header;

    }
    std::string StationSetup::searchColumn() const
    {
        return station()->valueToString();
    }

    std::unique_ptr<BaseEntity> StationSetup::cloneAsUnique()
    {
        return std::make_unique<StationSetup>();
    }

    void StationSetup::populateEntity()
    {
    }

    void StationSetup::afterMapping(BaseEntity&)
    {
    }

    void StationSetup::setTableName(const std::string table)
    {
        m_table_name = table;
    }


}
