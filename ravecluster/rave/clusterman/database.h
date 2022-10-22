#ifndef DATABASE_H
#define DATABASE_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{

    class Database : public BaseEntity
    {
    public:
        Database();
        ~Database() override;

        StringField* db_name() const;
        ForeignKeyField* server() const;
        IntegerField* db_port() const;

        void set_db_name(const std::string);
        void set_server(int);
        void set_db_port(int);

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
        StringField* m_db_name;
        IntegerField* m_db_port;
        ForeignKeyField* m_server;

        QStringList m_header;
        std::string m_table_name;
    };

}
#endif // DATABASE_H
