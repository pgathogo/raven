#ifndef STORAGEDISK_H
#define STORAGEDISK_H


#include "../../../framework/baseentity.h"

namespace ClusterManager
{
    class StorageDisk : public BaseEntity
    {
    public:
        StorageDisk();
        ~StorageDisk();

        StringField* disk_name() const;
        ForeignKeyField* audio_server();
        DecimalField* capacity();

        DecimalField* free_space();
        StringField* raw_disk_name();
        DateTimeField* last_refresh();

        void set_disk_name(const std::string);
        void set_audio_server(int);
        void set_capacity(double);

        void set_free_space(double);
        void set_raw_disk_name(const std::string);
        void set_last_refresh(QDateTime);

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
        StringField* m_disk_name;
        ForeignKeyField* m_audio_server;
        DecimalField* m_capacity;
        DecimalField* m_free_space;
        StringField* m_raw_disk_name;
        DateTimeField* m_last_refresh;

        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // STORAGEDISK_H
