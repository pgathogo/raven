#ifndef SERVER_H
#define SERVER_H

#include "../../../rave/framework/baseentity.h"

template<typename T>
class ChoiceField;

namespace ClusterManager
{

    class Server : public BaseEntity
    {
    public:
        Server();
        ~Server() override;

        StringField* server_name() const;
        StringField* server_ip() const;
        ChoiceField<std::string>* server_type() const;
        ForeignKeyField* cluster();

        void set_server_name(const std::string);
        void set_server_ip(const std::string);
        void set_server_type(const std::string);
        void set_cluster(int);

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
        StringField* m_server_name;
        StringField* m_server_ip;
        ChoiceField<std::string>*  m_server_type;
        ForeignKeyField* m_cluster;

        QStringList m_header;
        std::string m_table_name;
    };
}

#endif // SERVER_H
