#ifndef BRAND_H
#define BRAND_H

#pragma once


#include "../framework/baseentity.h"

class StringField;
class Client;

namespace TRAFFIK {

    class Brand : public BaseEntity
    {
    public:
        Brand();
        Brand(const std::shared_ptr<Client> client);
        ~Brand() override;

        void set_brand_name(std::string brand_name);
        StringField* brand_name() const;
        void set_client(int client_id);
        ForeignKeyField* client() const;
        void set_brand_manager(int brand_manager_id);
        ForeignKeyField* brand_manager() const;

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

    private:
        StringField* m_brand_name;
        ForeignKeyField* m_client;
        ForeignKeyField* m_brand_manager;

        QStringList mHeader;
        std::string mTableName;

    };
}

#endif // BRAND_H
