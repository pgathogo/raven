#ifndef USERACCESS_H
#define USERACCESS_H

#include "../../../framework/baseentity.h"

namespace ClusterManager
{
    class UserAccess : public BaseEntity
    {
    public:
        UserAccess();
        ~UserAccess();

        StringField* username() const;
        void set_username(const std::string);

        ForeignKeyField* station() const;
        void set_station(int);

        // Inherited
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
        StringField* m_username;
        ForeignKeyField* m_station;

        QStringList m_header;
        std::string m_table_name;
    };

}

#endif // USERACCESS_H
