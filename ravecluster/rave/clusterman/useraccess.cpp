#include "useraccess.h"

#include "station.h"

namespace ClusterManager
{
    UserAccess::UserAccess()
    {
        m_username = createField<StringField>("username", "Username");
        m_station = createField<ForeignKeyField>("station_id", "Station",
                                                 std::make_unique<Station>(),"station_name");

        m_header << QString::fromStdString(username()->fieldName())
                 << QString::fromStdString(station()->fieldName());

        setTableName("rave_useraccess");

    }

    UserAccess::~UserAccess()
    {

    }

    StringField* UserAccess::username() const
    {
        return m_username;

    }

    void UserAccess::set_username(const std::string name)
    {
      m_username->setValue(name);

    }

    ForeignKeyField* UserAccess::station() const
    {
      return m_station;

    }

    void UserAccess::set_station(int id)
    {

      m_station->setValue(id);
    }

    std::string UserAccess::tableName() const
    {
        return "rave_useraccess";
    }

    std::unique_ptr<BaseEntity> UserAccess::mapFields(StringMap*)
    {

    }

    std::vector<std::string> UserAccess::tableViewColumns() const
    {
        return tableViewCols<std::string>(username()->displayName(), station()->displayName());
    }

    std::vector<std::string> UserAccess::tableViewValues()
    {
        return {username()->displayName(), station()->displayName()};
    }

    QStringList UserAccess::tableHeaders() const
    {
        return m_header;

    }

    std::string UserAccess::searchColumn() const
    {
        return username()->valueToString();
    }

    std::shared_ptr<BaseEntity> UserAccess::cloneAsShared()
    {
        return std::make_shared<UserAccess>();
    }

    void UserAccess::populateEntity()
    {

    }

    void UserAccess::afterMapping(BaseEntity&)
    {

    }

    void UserAccess::setTableName(const std::string table)
    {
        m_table_name = table;

    }
}
