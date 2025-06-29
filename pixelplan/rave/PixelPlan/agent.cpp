#include <QDebug>
#include "agent.h"

Agent::Agent():
    m_name{}
    ,m_town{}
    ,m_telephone{}
    ,m_email{}
    ,mContactName{}
    ,mMobileNo{}
    ,mAddress{}
{

    m_name = createField<StringField>("agent_name", "Agents Name");
    m_name->setDBColumnName("name");
    m_name->setMandatory(true);

    m_town = createField<StringField>("town", "Town");
    m_telephone = createField<StringField>("telephone", "Telephone");
    m_email = createField<StringField>("email", "Email");
    mContactName = createField<StringField>("contact_name","Contact Name");
    mMobileNo = createField<StringField>("contact_mobile_no", "Mobile No");
    mAddress = createField<TextField>("address", "Address");

    mHeader << QString::fromStdString(m_name->fieldLabel())
            << QString::fromStdString(mContactName->fieldLabel())
            << QString::fromStdString(mMobileNo->fieldLabel());

    setTableName("rave_agent");


}

Agent::Agent(int)
{
    setTableName("rave_agent");
}

Agent::Agent(const Agent& /*other*/)
{
}

Agent::~Agent()
{
}

std::string Agent::tableName() const
{
    return mTableName;
}

void Agent::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> Agent::mapFields(StringMap* raw_entity)
{
    std::unique_ptr<Agent> agent = entityFieldMap<Agent>(raw_entity);
    return std::move(agent);
}

std::vector<std::string> Agent::tableViewColumns() const
{
    std::list<std::string> cols;

    return tableViewCols<std::string>(
                name()->displayName(),
                contactName()->displayName(),
                mobileNo()->displayName()
                );
}

std::vector<std::string> Agent::tableViewValues()
{
    return  tableViewColumns();
}

QStringList Agent::tableHeaders() const
{
    return mHeader;
}

std::string Agent::searchColumn() const
{
    return name()->valueToString();
}
void Agent::populateEntity()
{
}

StringField* Agent::name() const
{
    return m_name;
}
void Agent::setName(std::string pname)
{
    m_name->setValue(pname);
}

StringField* Agent::town() const
{
    return m_town;
}
void Agent::setTown(std::string ptown)
{
    m_town->setValue(ptown);
}

StringField* Agent::telephone() const
{
    return m_telephone;
}
void Agent::setTelephone(std::string ptele)
{
    m_telephone->setValue(ptele);
}

StringField* Agent::email() const
{
    return m_email;
}
void Agent::setEmail(std::string pemail)
{
    m_email->setValue(pemail);
}

StringField* Agent::contactName() const
{
    return mContactName;
}
void Agent::setContactName(std::string pcontact_name)
{
    mContactName->setValue(pcontact_name);
}

StringField* Agent::mobileNo() const
{
    return mMobileNo;
}
void Agent::setMobileNo(std::string pmobileno)
{
    mMobileNo->setValue(pmobileno);
}

TextField* Agent::address() const
{
    return mAddress;
}
void Agent::setAddress(std::string paddress)
{
    mAddress->setValue(paddress);
}

std::shared_ptr<BaseEntity> Agent::cloneAsShared()
{
    return std::make_shared<Agent>();
}

void Agent::afterMapping(BaseEntity &entity)
{

}

/*
std::string Agent::className()
{
    std::string cname = demangle(typeid(this).name());
    cname.erase(cname.end()-1);
    return cname;
}
*/

