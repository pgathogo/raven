#ifndef STATIONSETUP_H
#define STATIONSETUP_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{
    class StationSetup : public BaseEntity
    {
    public:
        StationSetup();
        ~StationSetup();

        ForeignKeyField* station() const;
        ForeignKeyField* audio_store() const;
        ForeignKeyField* audio_folder() const;
        ForeignKeyField* database() const;

        void set_station(int);
        void set_audio_store(int);
        void set_audio_folder(int);
        void set_database(int);

        std::string tableName() const override;
        std::unique_ptr<BaseEntity> mapFields(StringMap*) override;
        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;
        std::string searchColumn() const override;
        std::unique_ptr<BaseEntity> cloneAsUnique() override;

        void populateEntity() override;
        void afterMapping(BaseEntity&) override;
        void setTableName(const std::string) override;

    private:
        ForeignKeyField* m_station;
        ForeignKeyField* m_audio_store;
        ForeignKeyField* m_audio_folder;
        ForeignKeyField* m_database;

        QStringList m_header;
        std::string m_table_name;
    };


}

#endif // STATIONSETUP_H
