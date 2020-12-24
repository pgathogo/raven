#include "order.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"
#include "client.h"
#include "salesperson.h"
#include "brand.h"
#include "agent.h"
#include "orderpackage.h"

Order::Order()
{
    m_title = createField<StringField>("title", "Title");
    m_title->setMandatory(true);

    mOrderNumber = createField<IntegerField>("order_number", "Order Number");
    mOrderNumber->setReadOnly(true);

    mClient = createField<ForeignKeyField>("client_id", "Client",
                                    std::make_unique<Client>(), "name");

    mOrderDate = createField<DateField>("order_date", "Order Date");
    mStartDate = createField<DateField>("start_date", "Start Date");
    mEndDate = createField<DateField>("end_date", "End Date");

    mPackage = createField<ForeignKeyField>("package_id", "Package",
                                    std::make_unique<OrderPackage>(), "name");

    mRevenueType = createField<ChoiceField<std::string>>("revenue_type", "Revenue Type");
    mRevenueType->addChoice({"C", "Cash"});
    mRevenueType->addChoice({"T", "Trade"});

    mBillingType = createField<ChoiceField<std::string>>("billing_type", "Billing Type");
    mBillingType->addChoice({"G", "Gross"});
    mBillingType->addChoice({"N", "Net"});

    mBillingPeriod = createField<ChoiceField<std::string>>("billing_period", "Billing Period");
    mBillingPeriod->addChoice({"W", "Weekly"});
    mBillingPeriod->addChoice({"B", "Bi-Weekly"});
    mBillingPeriod->addChoice({"M", "Monthly"});

    mAccountRep = createField<ForeignKeyField>("account_rep_id", "Account Rep",
                                       std::make_unique<SalesPerson>(), "salesperson_name");

    mBrand = createField<ForeignKeyField>("brand_id", "Brand",
                                       std::make_unique<Brand>(), "brand_name");

    mAgency = createField<ForeignKeyField>("agency_id", "Agency",
                                       std::make_unique<Agent>(), "agent_name");

    mSpotsOrdered = createField<IntegerField>("spots_ordered", "Spots Ordered");
    mSpotsBooked = createField<IntegerField>("spots_booked", "Spots Booked");
    mSpotsPlayed = createField<IntegerField>("Spots_played", "Spots Played");
    mDiscount = createField<DecimalField>("discount", "Discount");

    mAgencyComm = createField<DecimalField>("agency_comm", "Agency Commission");

    mAgencyCommType = createField<ChoiceField<std::string>>("agency_comm_type", "Agency Comm Type");
    mAgencyCommType->addChoice({"F","Fixed"});
    mAgencyCommType->addChoice({"P","Percentage"});

    mSalesRepComm = createField<DecimalField>("sales_rep_comm", "Sales Rep Commission");

    mSalesRepCommType = createField<ChoiceField<std::string>>("sales_rep_comm_type", "Sales Rep Comm Type");
    mSalesRepCommType->addChoice({"F","Fixed"});
    mSalesRepCommType->addChoice({"P","Percentage"});

    mTradeCredit = createField<DecimalField>("trade_credit", "Trade Credit");

    mTradeCreditType = createField<ChoiceField<std::string>>("trade_credit_type", "Trade Credit Type");
    mTradeCreditType->addChoice({"F","Fixed"});
    mTradeCreditType->addChoice({"P","Percentage"});

    mLateFee = createField<DecimalField>("late_fee", "Late Fee");
    mGracePeriod = createField<IntegerField>("grace_period", "Grace Period");

    mBillingBasis = createField<ChoiceField<std::string>>("billing_basis", "Billing Basis");
    mBillingBasis->addChoice({"STD", "Standard"});
    mBillingBasis->addChoice({"DAY", "Daily"});
    mBillingBasis->addChoice({"PRD", "Billing Period"});

    mApprovalCount = createField<IntegerField>("approval_count", "Approval Count");
    mAddLogin = createField<StringField>("add_login", "Add Login");
    mAddDtime = createField<DateTimeField>("add_dtime", "Add DateTime");

    mHeader << QString::fromStdString(m_title->fieldLabel());
    setTableName("rave_order");
}

Order::Order(const Client* client)
{
    m_title = createField<StringField>("title", "Title");
    m_title->setMandatory(true);

    mOrderNumber = createField<IntegerField>("order_number", "Order Number");

    mClient = createField<ForeignKeyField>("client_id", "Client",
                                    std::make_unique<Client>(), "name");
    mClient->setValue(client->id());

    mOrderDate = createField<DateField>("order_date", "Order Date");
    mStartDate = createField<DateField>("start_date", "Start Date");
    mEndDate = createField<DateField>("end_date", "End Date");

    mPackage = createField<ForeignKeyField>("package_id", "Package",
                                    std::make_unique<OrderPackage>(), "name");

    mRevenueType = createField<ChoiceField<std::string>>("revenue_type", "Revenue Type");
    mRevenueType->addChoice({"C", "Cash"});
    mRevenueType->addChoice({"T", "Trade"});

    mBillingType = createField<ChoiceField<std::string>>("billing_type", "Billing Type");
    mBillingType->addChoice({"G", "Gross"});
    mBillingType->addChoice({"N", "Net"});

    mBillingPeriod = createField<ChoiceField<std::string>>("billing_period", "Billing Period");
    mBillingPeriod->addChoice({"W", "Weekly"});
    mBillingPeriod->addChoice({"B", "Bi-Weekly"});
    mBillingPeriod->addChoice({"M", "Monthly"});

    mAccountRep = createField<ForeignKeyField>("account_rep_id", "Account Rep",
                                       std::make_unique<SalesPerson>(), "salesperson_name");

    EntityDataModel edm;
    auto filter = std::make_tuple("client_id", "=", client->id());
    std::string fstr = edm.prepareFilter(filter);
    mBrand = createField<ForeignKeyField>("brand_id", "Brand",
                                       std::make_unique<Brand>(),
                                       "brand_name",
                                        fstr);

    mAgency = createField<ForeignKeyField>("agency_id", "Agency",
                                       std::make_unique<Agent>(), "agent_name");

    mSpotsOrdered = createField<IntegerField>("spots_ordered", "Spots Ordered");
    mSpotsBooked = createField<IntegerField>("spots_booked", "Spots Booked");
    mSpotsPlayed = createField<IntegerField>("Spots_played", "Spots Played");
    mDiscount = createField<DecimalField>("discount", "Discount");

    mAgencyComm = createField<DecimalField>("agency_comm", "Agency Commission");

    mAgencyCommType = createField<ChoiceField<std::string>>("agency_comm_type", "Agency Comm Type");
    mAgencyCommType->addChoice({"F","Fixed"});
    mAgencyCommType->addChoice({"P","Percentage"});

    mSalesRepComm = createField<DecimalField>("sales_rep_comm", "Sales Rep Commission");

    mSalesRepCommType = createField<ChoiceField<std::string>>("sales_rep_comm_type", "Sales Rep Comm Type");
    mSalesRepCommType->addChoice({"F","Fixed"});
    mSalesRepCommType->addChoice({"P","Percentage"});

    mTradeCredit = createField<DecimalField>("trade_credit", "Trade Credit");

    mTradeCreditType = createField<ChoiceField<std::string>>("trade_credit_type", "Trade Credit Type");
    mTradeCreditType->addChoice({"F","Fixed"});
    mTradeCreditType->addChoice({"P","Percentage"});

    mLateFee = createField<DecimalField>("late_fee", "Late Fee");
    mGracePeriod = createField<IntegerField>("grace_period", "Grace Period");

    mBillingBasis = createField<ChoiceField<std::string>>("billing_basis", "Billing Basis");
    mBillingBasis->addChoice({"STD", "Standard"});
    mBillingBasis->addChoice({"DAY", "Daily"});
    mBillingBasis->addChoice({"PRD", "Billing Period"});

    mApprovalCount = createField<IntegerField>("approval_count", "Approval Count");
    mAddLogin = createField<StringField>("add_login", "Add Login");
    mAddDtime = createField<DateTimeField>("add_dtime", "Add DateTime");

    mHeader << QString::fromStdString(m_title->fieldLabel());
    setTableName("rave_order");

}

Order::~Order()
{

}

StringField *Order::title() const
{
    return m_title;
}

IntegerField *Order::orderNumber() const
{
    return mOrderNumber;
}

ForeignKeyField *Order::client() const
{
    return mClient;
}

DateField *Order::orderDate() const
{
    return mOrderDate;
}

DateField *Order::startDate() const
{
    return mStartDate;
}

DateField *Order::endDate() const
{
    return mEndDate;
}

ForeignKeyField *Order::package() const
{
    return mPackage;
}

ChoiceField<std::string> *Order::revenueType() const
{
    return mRevenueType;
}

ChoiceField<std::string> *Order::billingType() const
{
    return mBillingType;
}

ChoiceField<std::string> *Order::billingPeriod() const
{
    return mBillingPeriod;
}

ForeignKeyField *Order::accountRep() const
{
    return mAccountRep;
}

ForeignKeyField *Order::brand() const
{
    return mBrand;
}

ForeignKeyField *Order::agency() const
{
    return mAgency;
}

IntegerField *Order::spotsOrdered() const
{
    return mSpotsOrdered;
}

IntegerField *Order::spotsBooked() const
{
    return mSpotsBooked;
}

IntegerField *Order::spotsPlayed() const
{
    return mSpotsPlayed;
}

DecimalField *Order::discount() const
{
    return mDiscount;
}

DecimalField *Order::agencyComm() const
{
    return mAgencyComm;
}

ChoiceField<std::string> *Order::agencyCommType() const
{
    return mAgencyCommType;
}

DecimalField *Order::salesRepComm() const
{
    return mSalesRepComm;
}

ChoiceField<std::string> *Order::salesRepCommType() const
{
    return mSalesRepCommType;
}

DecimalField *Order::tradeCredit() const
{
    return mTradeCredit;
}

ChoiceField<std::string> *Order::tradeCreditType() const
{
    return mTradeCreditType;
}

DecimalField *Order::lateFee() const
{
    return mLateFee;
}

IntegerField *Order::gracePeriod() const
{
    return mGracePeriod;
}

ChoiceField<std::string> *Order::billingBasis() const
{
    return mBillingBasis;
}

IntegerField *Order::approvalCount() const
{
    return mApprovalCount;
}

StringField *Order::addLogin() const
{
    return mAddLogin;
}

DateTimeField *Order::addDtime() const
{
    return mAddDtime;
}


void Order::setTitle(const std::string val)
{
    m_title->setValue(val);
}

void Order::setOrderNumber(int i)
{
    mOrderNumber->setValue(i);

}

void Order::setClient(int i)
{
    mClient->setValue(i);
}

void Order::setOrderDate(QDate dt)
{
    mOrderDate->setValue(dt);
}

void Order::setStartDate(QDate dt)
{
    mStartDate->setValue(dt);
}

void Order::setEndDate(QDate dt)
{
    mEndDate->setValue(dt);
}

void Order::setPackage(int i)
{
    mPackage->setValue(i);

}

void Order::setRevenueType(const std::string val)
{
    mRevenueType->setValue(val);
}

void Order::setBillingType(const std::string val)
{
    mBillingType->setValue(val);
}

void Order::setBillingPeriod(const std::string val)
{
    mBillingPeriod->setValue(val);
}

void Order::setAccountRep(int i)
{
    mAccountRep->setValue(i);
}

void Order::setBrand(int i)
{
    mBrand->setValue(i);
}

void Order::setAgency(int i)
{
    mAgency->setValue(i);
}

void Order::setSpotsOrdered(int i)
{
    mSpotsOrdered->setValue(i);
}

void Order::setSpotsBooked(int i)
{
    mSpotsBooked->setValue(i);
}

void Order::setSpotsPlayed(int i)
{
    mSpotsPlayed->setValue(i);
}

void Order::setDiscount(double d)
{
    mDiscount->setValue(d);
}

void Order::setAgencyComm(double d)
{
    mAgencyComm->setValue(d);

}

void Order::setAgencyCommType(const std::string val)
{
    mAgencyCommType->setValue(val);
}

void Order::setSalesRepComm(double d)
{
    mSalesRepComm->setValue(d);
}

void Order::setSalesRepCommType(const std::string val)
{
    mSalesRepCommType->setValue(val);
}

void Order::setTradeCredit(double d)
{
    mTradeCredit->setValue(d);
}

void Order::setTradeCreditType(const std::string val)
{
    mTradeCreditType->setValue(val);
}

void Order::setLateFee(double d)
{
    mLateFee->setValue(d);
}

void Order::setGracePeriod(int i)
{
    mGracePeriod->setValue(i);
}

void Order::setBillingBasis(const std::string val)
{
    mBillingBasis->setValue(val);
}

void Order::setApprovalCount(int i)
{
    mApprovalCount->setValue(i);
}

void Order::setAddLogin(const std::string val)
{
    mAddLogin->setValue(val);
}

void Order::setAddDtime(QDateTime dt)
{
    mAddDtime->setValue(dt);

}

std::string Order::tableName() const
{
    return mTableName;
}

void Order::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> Order::mapFields(StringMap *raw_entity)
{
}

std::vector<std::string> Order::tableViewColumns() const
{
    return tableViewCols<std::string>(title()->displayName());
}

std::vector<std::string> Order::tableViewValues()
{
    return {title()->displayName()};
}

QStringList Order::tableHeaders() const
{
    return mHeader;
}

std::string Order::searchColumn() const
{
    return title()->valueToString();
}

void Order::populateEntity()
{
}

std::unique_ptr<BaseEntity> Order::cloneAsUnique()
{
    return std::make_unique<Order>();
}

void Order::afterMapping(BaseEntity &entity)
{

}

ActionResult Order::validate()
{
    if (mStartDate->value() < mOrderDate->value()){
        return std::make_tuple(ActionResultType::arERROR,
                               "Start date less than Order Date!");
    }

    if (mEndDate->value() < mStartDate->value()){
        return std::make_tuple(ActionResultType::arERROR,
                               "End Date less than Order Date!");
    }

    if (mSpotsOrdered->value() == 0 ){
        return std::make_tuple(ActionResultType::arERROR,
                               "No spots ordered!");
    }

    return BaseEntity::validate();
}
