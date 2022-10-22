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

        void set_disk_name(const std::string);
        void set_audio_server(int);
        void set_capacity(double);

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
        StringField* m_disk_name;
        ForeignKeyField* m_audio_server;
        DecimalField* m_capacity;

        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // STORAGEDISK_H
