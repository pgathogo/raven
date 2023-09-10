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
        Server& operator=(const Server&);

        ~Server() override;



        StringField* server_name() const;
        StringField* server_ip() const;
        IntegerField* port_no() const;
        ChoiceField<std::string>* server_type() const;
        ForeignKeyField* cluster() const;
        StringField* db_admin() const;
        StringField* db_admin_password() const;

        void set_server_name(const std::string);
        void set_server_ip(const std::string);
        void set_port_no(int);
        void set_server_type(const std::string);
        void set_cluster(int);
        void set_db_admin(const std::string);
        void set_db_admin_password(const std::string);

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
        StringField* m_server_name;
        StringField* m_server_ip;
        IntegerField* m_port_no;
        ChoiceField<std::string>*  m_server_type;
        ForeignKeyField* m_cluster;
        StringField* m_db_admin;
        StringField* m_db_admin_password;

        QStringList m_header;
        std::string m_table_name;
    };
}

#endif // SERVER_H
