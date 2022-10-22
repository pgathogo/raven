#ifndef AUDIOSTORE_H
#define AUDIOSTORE_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{
    class AudioStore : public BaseEntity
    {
    public:
        AudioStore();
        ~AudioStore();

        StringField* store_name() const;
        ForeignKeyField* server() const;
        ForeignKeyField* station() const;
        TextField* notes() const;

        void set_store_name(const std::string);
        void set_server(int);
        void set_station(int);
        void set_notes(const std::string);

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
      StringField* m_store_name;
      ForeignKeyField* m_server;
      ForeignKeyField* m_station;
      TextField* m_notes;

      QStringList m_header;
      std::string m_table_name;
    };
}
#endif // AUDIOSTORE_H
