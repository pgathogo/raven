#include "userconfig.h"

namespace SECURITY
{
    UserConfig::UserConfig()
    {
        m_username = createField<StringField>("username", "Username");
        m_reset_password = createField<IntegerField>("reset_password", "Reset Password?");

        m_header << QString::fromStdString(m_username->fieldLabel());
        setTableName("rave_userconfig");
    }

    UserConfig::~UserConfig()
    {

    }

    StringField* UserConfig::username() const
    {
        return m_username;
    }

    IntegerField* UserConfig::reset_password()
    {
        return m_reset_password;
    }

    void UserConfig::set_username(std::string uname)
    {
        m_username->setValue(uname);

    }

    void UserConfig::set_reset_password(int flag)
    {
        m_reset_password->setValue(flag);

    }

    std::string UserConfig::tableName() const
    {
        return m_table_name;

    }

    std::unique_ptr<BaseEntity> UserConfig::mapFields(StringMap*)
    {
    }

    std::vector<std::string> UserConfig::tableViewColumns() const
    {
        return tableViewCols<std::string>(username()->displayName());
    }

    std::vector<std::string> UserConfig::tableViewValues()
    {
        return {username()->displayName()};
    }

    QStringList UserConfig::tableHeaders() const
    {
        return m_header;
    }

    std::string UserConfig::searchColumn() const
    {
        return username()->valueToString();
    }

    std::shared_ptr<BaseEntity> UserConfig::cloneAsShared()
    {
        return std::make_shared<UserConfig>();

    }

    void UserConfig::populateEntity()
    {
    }

    void UserConfig::afterMapping(BaseEntity&)
    {

    }

    void UserConfig::setTableName(const std::string table)
    {
        m_table_name = table;
    }



}
