#ifndef AUDIOFOLDER_H
#define AUDIOFOLDER_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{
    class AudioFolder : public BaseEntity
    {
    public:
        AudioFolder();
        ~AudioFolder();

        StringField* folder_name() const;
        ForeignKeyField* disk();

        void set_folder_name(const std::string);
        void set_disk(int);

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
        StringField* m_folder_name;
        ForeignKeyField* m_disk;

        QStringList m_header;
        std::string m_table_name;
    };
}

#endif // AUDIOFOLDER_H
