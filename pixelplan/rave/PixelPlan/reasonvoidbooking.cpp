
#include "reasonvoidbooking.h"

namespace PIXELPLAN
{

    ReasonVoidBooking::ReasonVoidBooking()
    :m_reason{nullptr}
    ,m_description{nullptr}
    {
        m_reason = createField<StringField>("reason", "Void Reason");
        m_description = createField<TextField>("description", "Description");

        m_header << QString::fromStdString(m_reason->fieldLabel())
                << QString::fromStdString(m_description->fieldLabel());

        setTableName("rave_reasonvoidbooking");

    }

    ReasonVoidBooking::~ReasonVoidBooking()
    {}

    StringField* ReasonVoidBooking::reason()
    {
        return m_reason;
    }


    void ReasonVoidBooking::set_reason(const std::string res)
    {
        m_reason->setValue(res);
    }

    TextField* ReasonVoidBooking::description()
    {
        return m_description;
    }

    void ReasonVoidBooking::set_description(const std::string desc)
    {
        m_description->setValue(desc);
    }

    std::unique_ptr<BaseEntity> ReasonVoidBooking::mapFields(StringMap* e)
    {
    }

    std::vector<std::string> ReasonVoidBooking::tableViewColumns() const
    {
        return tableViewCols<std::string>(
            m_reason->displayName(),
            m_description->displayName()
            );
    }

    std::vector<std::string> ReasonVoidBooking::tableViewValues()
    {
        return tableViewCols<std::string>(
            m_reason->valueToString(),
            m_description->valueToString()
            );

    }

    QStringList ReasonVoidBooking::tableHeaders() const
    {
        return m_header;
    }

    std::string ReasonVoidBooking::tableName() const
    {
        return m_table_name;
    }

    void ReasonVoidBooking::setTableName(const std::string table_name)
    {
        m_table_name = table_name;

    }

    std::string ReasonVoidBooking::searchColumn() const
    {
        return m_reason->valueToString();

    }

    void ReasonVoidBooking::populateEntity()
    {

    }

    std::shared_ptr<BaseEntity> ReasonVoidBooking::cloneAsShared()
    {
        return std::make_shared<PIXELPLAN::ReasonVoidBooking>();

    }

    void ReasonVoidBooking::afterMapping(BaseEntity& entity) {}



}
