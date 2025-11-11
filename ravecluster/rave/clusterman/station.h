#ifndef STATION_H
#define STATION_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{

    class Station : public BaseEntity
    {
    public:
        Station();
        Station& operator=(const Station&);

        ~Station();

        StringField* station_name() const;
        StringField* db_name() const;
        ForeignKeyField* cluster() const;
        StringField* station_code() const;

        void set_station_name(const std::string);
        void set_db_name(const std::string);
        void set_cluster(int);
        void set_station_code(const std::string);

        std::string tableName() const override;
        std::unique_ptr<BaseEntity> mapFields(StringMap*) override;
        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;
        std::string searchColumn() const override;
        std::shared_ptr<BaseEntity> cloneAsShared() override;

        void populateEntity() override;
        void afterMapping(BaseEntity&) override;
        void setTableName(const std::string) override;

    private:
        StringField* m_station_name;
        StringField* m_db_name;
        ForeignKeyField* m_cluster;
        StringField* m_station_code;


        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // STATION_H
