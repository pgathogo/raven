#include "client.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"

#include "agent.h"
#include "clientgroup.h"
#include "salesperson.h"


Client::Client()
{
    mName = createField<StringField>("name", "Client Name:");
    mName->setMandatory(true);

    mBillName = createField<StringField>("bill_name", "Billing Name:");
    mAddress1 = createField<StringField>("address1", "Address1:");
    mAddress2 = createField<StringField>("address2", "Address2:");
    mTown = createField<StringField>("town", "Town:");
    mPostalNo = createField<StringField>("postal_no", "Postal No.");
    mPostalCode = createField<StringField>("postal_code", "Postal Code:");
    mTelephone = createField<StringField>("telephone", "Telephone");
    mClientEmail = createField<StringField>("client_email", "Client Email");
    mClientMobile = createField<StringField>("client_mobile", "Client Mobile");
    mContactName = createField<StringField>("contact_name", "Contact Name");
    mContactEmail = createField<StringField>("contact_email", "Contact Email");
    mContactMobile = createField<StringField>("contact_mobile", "Contact Mobile");
    mGracePeriod = createField<IntegerField>("grace_period", "Grace Period");
    mDiscountPercent = createField<DecimalField>("discount_percent", "Discount Percentage");
    mAgencyComm = createField<DecimalField>("agency_comm", "Agency Commission");
    mSaleRepComm = createField<DecimalField>("sales_rep_comm", "Sales Rep. Commission");
    mInterestRate = createField<DecimalField>("interest_rate", "Interest Rate");
    mLateFee = createField<DecimalField>("late_fee", "Late Fee");

    mAgency = createField<ForeignKeyField>( "agency_id", "Agency",
                                            std::make_unique<Agent>(), "agent_name");
    mClientGroup = createField<ForeignKeyField>("client_group_id", "Client Group",
                                                std::make_unique<ClientGroup>(), "group_name" );
    mAccountManager = createField<ForeignKeyField>("account_manager_id", "Account Manager",
                                                   std::make_unique<SalesPerson>(), "salesperson_name");

    mContactSalute = createField<ChoiceField<std::string>>("contact_salute", "Contact Salute");
    mContactSalute->addChoice({"Miss","Miss."});
    mContactSalute->addChoice({"MS","Ms."});
    mContactSalute->addChoice({"MRS","Mrs."});
    mContactSalute->addChoice({"MR","Mr."});

    mRevenueType = createField<ChoiceField<std::string>>("revenue_type", "Revenue Type");
    mRevenueType->addChoice({"C", "Cash"});
    mRevenueType->addChoice({"T", "Trade"});

    mBillCycle = createField<ChoiceField<std::string>>("bill_cycle", "Billing Cycle");
    mBillCycle->addChoice({"W", "Weekly"});
    mBillCycle->addChoice({"B", "Bi-Weekly"});
    mBillCycle->addChoice({"M", "Monthly"});

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mTelephone->fieldLabel())
            << QString::fromStdString(mContactName->fieldLabel())
            << QString::fromStdString(mContactMobile->fieldLabel());

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
    return tableViewCols<std::string>(
                name()->displayName(),
                telephone()->displayName(),
                contactName()->displayName(),
                contactMobile()->displayName());
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
    return mName;
}
void Client::setName(const std::string n)
{
    mName->setValue(n);
}
StringField* Client::telephone() const
{
    return mTelephone;
}
void Client::setTelephone(const std::string tele)
{
    mTelephone->setValue(tele);
}
StringField* Client::contactName() const
{
    return mContactName;
}
void Client::setContactName(const std::string contName)
{
    mContactName->setValue(contName);
}
StringField* Client::contactMobile() const
{
    return mContactMobile;
}
void Client::setContactMobile(const std::string contMobile) const
{
    mContactMobile->setValue(contMobile);
}

StringField* Client::billName() const
{
    return  mBillName;
}
StringField* Client::address1() const
{
    return  mAddress1;
}
StringField* Client::address2() const
{
    return  mAddress2;
}
StringField* Client::town() const
{
    return  mTown;
}
StringField* Client::postalNo() const
{
    return  mPostalNo;
}
StringField* Client::postalCode() const
{
    return  mPostalCode;
}
StringField* Client::clientMobile() const
{
    return mClientMobile;
}
StringField* Client::clientEmail() const
{
    return  mClientEmail;
}
ChoiceField<std::string>* Client::contactSalute() const
{
    return  mContactSalute;
}
StringField* Client::contactEmail() const
{
    return  mContactEmail;
}

ForeignKeyField* Client::agency() const
{
    return  mAgency;
}

ForeignKeyField* Client::clientGroup() const
{
    return  mClientGroup;
}

ForeignKeyField* Client::accountManager() const
{
    return  mAccountManager;
}

ChoiceField<std::string>* Client::revenueType() const
{
    return  mRevenueType;
}
DecimalField* Client::discountPercent() const
{
    return  mDiscountPercent;
}
DecimalField* Client::agencyComm () const
{
    return  mAgencyComm ;
}
DecimalField* Client::saleRepComm() const
{
    return  mSaleRepComm;
}
DecimalField* Client::interestRate() const
{
    return  mInterestRate;
}
DecimalField* Client::lateFee() const
{
    return  mLateFee;
}
IntegerField* Client::gracePeriod() const
{
    return  mGracePeriod;
}
ChoiceField<std::string>* Client::billCycle() const
{
    return  mBillCycle;
}

void Client::setBillName(const std::string bn)
{
    mBillName->setValue(bn);
}

void Client::setAddress1(const std::string address1)
{
    mAddress1->setValue(address1);
}

void Client::setAddress2(const std::string address2)
{
    mAddress2->setValue(address2);
}

void Client::setTown(const std::string town)
{
    mTown->setValue(town);
}

void Client::setPostalNo(const std::string pNo)
{
    mPostalNo->setValue(pNo);
}

void Client::setPostalCode(const std::string pCode)
{
    mPostalCode->setValue(pCode);
}

void Client::setClientEmail(const std::string cEmail)
{
    mClientEmail->setValue(cEmail);
}

void Client::setClientMobile(const std::string cMobile)
{
    mClientMobile->setValue(cMobile);
}
void Client::setContactSalute(const std::string cSalute)
{
    mContactSalute->setValue(cSalute);
}
void Client::setContactEmail(const std::string contEmail)
{
    mContactEmail->setValue(contEmail);
}

void Client::setContactMobile(const std::string contMobile)
{
    mContactMobile->setValue(contMobile);
}

void Client::setAgency(int agent)
{
    mAgency->setValue(agent);
}

void Client::setClientGroup(int cGroup)
{
    mClientGroup->setValue(cGroup);
}

void Client::setAccountManager(int aManager)
{
    mAccountManager->setValue(aManager);
}

void Client::setRevenueType(const std::string rType)
{
    mRevenueType->setValue(rType);
}

void Client::setDiscountPercent(double dPercent)
{
    mDiscountPercent->setValue(dPercent);
}

void Client::setAgencyComm(double aComm )
{
    mAgencyComm->setValue(aComm);
}

void Client::setSaleRepComm(double sRepComm)
{
    mSaleRepComm->setValue(sRepComm);
}

void Client::setInterestRate(double iRate)
{
    mInterestRate->setValue(iRate);
}

void Client::setLateFee(double lFee)
{
    mLateFee->setValue(lFee);
}

void Client::setGracePeriod(int gPeriod)
{
    mGracePeriod->setValue(gPeriod);
}

void Client::setBillCycle(const std::string bCycle)
{
    mBillCycle->setValue(bCycle);
}

std::unique_ptr<BaseEntity> Client::cloneAsUnique()
{
    return std::make_unique<Client>();
}

void Client::afterMapping(BaseEntity &entity)
{

}
