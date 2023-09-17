#ifndef USERCONFIG_H
#define USERCONFIG_H

#include "../../../framework/baseentity.h"

namespace SECURITY
{
    class UserConfig : public BaseEntity
    {
    public:
        UserConfig();
        ~UserConfig();

        StringField* username() const;
        IntegerField* reset_password();

        void set_username(std::string);
        void set_reset_password(int);

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
        IntegerField* m_reset_password;

        QStringList m_header;
        std::string m_table_name;
    };

}
#endif // USERCONFIG_H
