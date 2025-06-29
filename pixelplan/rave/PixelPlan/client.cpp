#include "client.h"
#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/entitydatamodel.h"

#include "agent.h"
#include "clientgroup.h"
#include "salesperson.h"


Client::Client()
{
    m_name = createField<StringField>("name", "Client Name:");
    m_name->setMandatory(true);

    m_biil_name = createField<StringField>("bill_name", "Billing Name:");
    m_address1 = createField<StringField>("address1", "Address1:");
    m_address2 = createField<StringField>("address2", "Address2:");
    m_town = createField<StringField>("town", "Town:");
    m_postal_no = createField<StringField>("postal_no", "Postal No.");
    m_postal_code = createField<StringField>("postal_code", "Postal Code:");
    m_telephone = createField<StringField>("telephone", "Telephone");
    m_client_email = createField<StringField>("client_email", "Client Email");
    m_client_mobile = createField<StringField>("client_mobile", "Client Mobile");
    m_contact_name = createField<StringField>("contact_name", "Contact Name");
    m_contact_email = createField<StringField>("contact_email", "Contact Email");
    m_contact_mobile = createField<StringField>("contact_mobile", "Contact Mobile");
    m_grace_period = createField<IntegerField>("grace_period", "Grace Period");
    m_discount_percent = createField<DecimalField>("discount_percent", "Discount Percentage");
    m_agency_comm = createField<DecimalField>("agency_comm", "Agency Commission");
    m_sale_rep_comm = createField<DecimalField>("sales_rep_comm", "Sales Rep. Commission");
    m_interest_rate = createField<DecimalField>("interest_rate", "Interest Rate");
    m_late_fee = createField<DecimalField>("late_fee", "Late Fee");

    m_agency = createField<ForeignKeyField>( "agency_id", "Agency",
                                            std::make_unique<Agent>(), "agent_name");
    m_client_group = createField<ForeignKeyField>("client_group_id", "Client Group",
                                                std::make_unique<ClientGroup>(), "group_name" );
    m_account_manager = createField<ForeignKeyField>("account_manager_id", "Account Manager",
                                                   std::make_unique<SalesPerson>(), "salesperson_name");

    m_contact_salute = createField<ChoiceField<std::string>>("contact_salute", "Contact Salute");
    m_contact_salute->addChoice({"Miss","Miss."});
    m_contact_salute->addChoice({"MS","Ms."});
    m_contact_salute->addChoice({"MRS","Mrs."});
    m_contact_salute->addChoice({"MR","Mr."});

    m_revenue_type = createField<ChoiceField<std::string>>("revenue_type", "Revenue Type");
    m_revenue_type->addChoice({"C", "Cash"});
    m_revenue_type->addChoice({"T", "Trade"});

    m_bill_cycle = createField<ChoiceField<std::string>>("bill_cycle", "Billing Cycle");
    m_bill_cycle->addChoice({"W", "Weekly"});
    m_bill_cycle->addChoice({"B", "Bi-Weekly"});
    m_bill_cycle->addChoice({"M", "Monthly"});

    mHeader << QString::fromStdString(m_name->fieldLabel())
            << QString::fromStdString(m_telephone->fieldLabel())
            << QString::fromStdString(m_contact_name->fieldLabel())
            << QString::fromStdString(m_contact_mobile->fieldLabel());

    setTableName("rave_client");
}

Client::Client(int)
{
    setTableName("rave_client");
}

Client::~Client()
{
}

std::string Client::tableName() const
{
    return "rave_client";
}
void Client::setTableName(const std::string table_name)
{
    mTableName = table_name;
}
std::unique_ptr<BaseEntity> Client::mapFields(StringMap* raw_entity)
{
    std::unique_ptr<Client> client = entityFieldMap<Client>(raw_entity);
    return std::move(client);
}

std::vector<std::string> Client::tableViewColumns() const
{
    name()->set_display_width(300);
    telephone()->set_display_width(200);
    contact_name()->set_display_width(200);
    contact_mobile()->set_display_width(200);

    return tableViewCols<std::string>(
                name()->displayName(),
                telephone()->displayName(),
                contact_name()->displayName(),
                contact_mobile()->displayName());
}

std::vector<std::string> Client::tableViewValues()
{
    return tableViewColumns();

}

QStringList Client::tableHeaders() const
{
    return mHeader;
}

std::string Client::searchColumn() const
{
    return name()->valueToString();
}
void Client::populateEntity()
{
}
StringField* Client::name() const
{
    return m_name;
}
void Client::set_name(const std::string n)
{
    m_name->setValue(n);
}
StringField* Client::telephone() const
{
    return m_telephone;
}
void Client::set_telepone(const std::string tele)
{
    m_telephone->setValue(tele);
}
StringField* Client::contact_name() const
{
    return m_contact_name;
}
void Client::set_contact_name(const std::string contName)
{
    m_contact_name->setValue(contName);
}
StringField* Client::contact_mobile() const
{
    return m_contact_mobile;
}
void Client::set_contact_mobile(const std::string contMobile) const
{
    m_contact_mobile->setValue(contMobile);
}

StringField* Client::bill_name() const
{
    return  m_biil_name;
}
StringField* Client::address1() const
{
    return  m_address1;
}
StringField* Client::address2() const
{
    return  m_address2;
}
StringField* Client::town() const
{
    return  m_town;
}
StringField* Client::postalNo() const
{
    return  m_postal_no;
}
StringField* Client::postal_code() const
{
    return  m_postal_code;
}
StringField* Client::client_mobile() const
{
    return m_client_mobile;
}
StringField* Client::client_email() const
{
    return  m_client_email;
}
ChoiceField<std::string>* Client::contact_salute() const
{
    return  m_contact_salute;
}
StringField* Client::contact_email() const
{
    return  m_contact_email;
}

ForeignKeyField* Client::agency() const
{
    return  m_agency;
}

ForeignKeyField* Client::group_manager() const
{
    return  m_client_group;
}

ForeignKeyField* Client::account_manager() const
{
    return  m_account_manager;
}

ChoiceField<std::string>* Client::revenue_type() const
{
    return  m_revenue_type;
}
DecimalField* Client::discount_percent() const
{
    return  m_discount_percent;
}
DecimalField* Client::agency_comm () const
{
    return  m_agency_comm ;
}
DecimalField* Client::sale_rep_comm() const
{
    return  m_sale_rep_comm;
}
DecimalField* Client::interest_rate() const
{
    return  m_interest_rate;
}
DecimalField* Client::late_fee() const
{
    return  m_late_fee;
}
IntegerField* Client::grace_period() const
{
    return  m_grace_period;
}
ChoiceField<std::string>* Client::bill_cycle() const
{
    return  m_bill_cycle;
}

void Client::set_bill_name(const std::string bn)
{
    m_biil_name->setValue(bn);
}

void Client::set_address1(const std::string address1)
{
    m_address1->setValue(address1);
}

void Client::set_address2(const std::string address2)
{
    m_address2->setValue(address2);
}

void Client::set_town(const std::string town)
{
    m_town->setValue(town);
}

void Client::set_postal_no(const std::string pNo)
{
    m_postal_no->setValue(pNo);
}

void Client::set_postal_code(const std::string pCode)
{
    m_postal_code->setValue(pCode);
}

void Client::set_client_email(const std::string cEmail)
{
    m_client_email->setValue(cEmail);
}

void Client::set_client_mobile(const std::string cMobile)
{
    m_client_mobile->setValue(cMobile);
}
void Client::set_contact_salute(const std::string cSalute)
{
    m_contact_salute->setValue(cSalute);
}
void Client::set_contact_email(const std::string contEmail)
{
    m_contact_email->setValue(contEmail);
}

void Client::set_contact_mobile(const std::string contMobile)
{
    m_contact_mobile->setValue(contMobile);
}

void Client::set_agency(int agent)
{
    m_agency->setValue(agent);
}

void Client::set_client_group(int cGroup)
{
    m_client_group->setValue(cGroup);
}

void Client::set_account_manager(int aManager)
{
    m_account_manager->setValue(aManager);
}

void Client::set_revenue_type(const std::string rType)
{
    m_revenue_type->setValue(rType);
}

void Client::set_discount_percent(double dPercent)
{
    m_discount_percent->setValue(dPercent);
}

void Client::set_agency_comm(double aComm )
{
    m_agency_comm->setValue(aComm);
}

void Client::set_sale_rep_comm(double sRepComm)
{
    m_sale_rep_comm->setValue(sRepComm);
}

void Client::set_interest_rate(double iRate)
{
    m_interest_rate->setValue(iRate);
}

void Client::set_late_fee(double lFee)
{
    m_late_fee->setValue(lFee);
}

void Client::set_grace_period(int gPeriod)
{
    m_grace_period->setValue(gPeriod);
}

void Client::set_bill_cycle(const std::string bCycle)
{
    m_bill_cycle->setValue(bCycle);
}

std::shared_ptr<BaseEntity> Client::cloneAsShared()
{
    return std::make_shared<Client>();
}

void Client::afterMapping(BaseEntity &entity)
{

}
