#include "traffiksetup.h"
#include "../framework/choicefield.h"
#include "../utils/tools.h"

TraffikSetup::TraffikSetup()
{
    mStationName = createField<StringField>("station_name", "Station Name");
    mStationLogo = createField<StringField>("station_logo","Station Logo");
    mAddress1 = createField<StringField>("address1", "Address 1");
    mAddress2 = createField<StringField>("address2", "Address 2");
    mMaxBreakSpots = createField<IntegerField>("max_spot_per_break", "Max Spots");
    mAgencyComm = createField<DecimalField>("agency_comm", "Agency Commision");

    mAgencyCommType = createField<ChoiceField<std::string>>("agency_comm_type", "Agency Commision Type");
    mAgencyCommType->addChoice({"P", "Percentage"});
    mAgencyCommType->addChoice({"F", "Fixed"});

    mSaleRepComm = createField<DecimalField>("sales_rep_comm", "Sale Rep Commission");
    mSaleRepCommType = createField<ChoiceField<std::string>>("sales_rep_comm_type", "Sales Commission Type");
    mSaleRepCommType->addChoice({"P", "Percentage"});
    mSaleRepCommType->addChoice({"F", "Fixed"});

    mBillingCycle = createField<ChoiceField<std::string>>("billing_cycle", "Billing Cycle");
    mBillingCycle->addChoice({"W", "Weekly"});
    mBillingCycle->addChoice({"B", "Bi-Weekly"});
    mBillingCycle->addChoice({"M", "Monthly"});

    mLateFee = createField<DecimalField>("late_fee", "Late Fee");
    mInterestRate = createField<DecimalField>("interest_rate", "Interest Rate");
    mGracePeriod = createField<IntegerField>("pay_grace_period", "Grace Period");
    mOrderApprovalLevels = createField<IntegerField>("order_approval_levels", "Approval Levels");

    mHeader << stoq(mStationName->fieldName());
    setTableName("rave_setup");

}

TraffikSetup::~TraffikSetup()
{
}

StringField *TraffikSetup::stationName() const
{
    return mStationName;
}

void TraffikSetup::setStationName(const std::string station_name)
{
    mStationName->setValue( station_name );
}

StringField *TraffikSetup::stationLogo() const
{
    return mStationLogo;
}

void TraffikSetup::setStationLogo(const std::string station_logo)
{
    mStationLogo->setValue( station_logo );
}

StringField *TraffikSetup::address1() const
{
    return mAddress1;
}

void TraffikSetup::setAddress1(const std::string addr1)
{
    mAddress1->setValue( addr1 );
}

StringField *TraffikSetup::address2() const
{
    return mAddress2;
}

void TraffikSetup::setAddress2(const std::string addr2)
{
    mAddress2->setValue( addr2 );
}

IntegerField *TraffikSetup::maxBreakSpots() const
{
    return mMaxBreakSpots;
}

void TraffikSetup::setMaxBreakSpots(int max_spots)
{
    mMaxBreakSpots->setValue( max_spots );
}

DecimalField *TraffikSetup::agencyComm() const
{
    return mAgencyComm;
}

void TraffikSetup::setAgencyComm(double aComm)
{
    mAgencyComm->setValue( aComm );
}

ChoiceField<std::string> *TraffikSetup::agencyCommType() const
{
    return mAgencyCommType;
}

void TraffikSetup::setAgencyCommType(const std::string comm_type)
{
    mAgencyCommType->setValue( comm_type );
}

DecimalField *TraffikSetup::saleRepComm() const
{
    return mSaleRepComm;
}

void TraffikSetup::setSaleRepComm(double rep_comm)
{
    mSaleRepComm->setValue( rep_comm );

}

ChoiceField<std::string> *TraffikSetup::saleRepCommType() const
{
    return mSaleRepCommType;
}

void TraffikSetup::setSaleRepCommType(const std::string rep_type)
{
    mSaleRepCommType->setValue( rep_type );
}

ChoiceField<std::string> *TraffikSetup::billingCycle() const
{
    return mBillingCycle;
}

void TraffikSetup::setBillingCycle(const std::string bill_cycle)
{
    mBillingCycle->setValue( bill_cycle );
}

DecimalField *TraffikSetup::lateFee() const
{
    return mLateFee;
}

void TraffikSetup::setLateFee(double late_fee)
{
    mLateFee->setValue( late_fee );
}

DecimalField *TraffikSetup::interestRate() const
{
    return mInterestRate;
}

void TraffikSetup::setInterestRate(double int_rate)
{
    mInterestRate->setValue( int_rate );
}

IntegerField *TraffikSetup::gracePeriod() const
{
    return mGracePeriod;
}

void TraffikSetup::setGracePeriod(int grace_period)
{
    mGracePeriod->setValue( grace_period );
}

IntegerField *TraffikSetup::orderApprovalLevels() const
{
    return mOrderApprovalLevels;
}

void TraffikSetup::setOrderApprovalLevels(int aprv_levels)
{
    mOrderApprovalLevels->setValue(aprv_levels);
}

std::string TraffikSetup::tableName() const
{
    return mTableName;
}

void TraffikSetup::setTableName(const std::string table_name)
{
   mTableName = table_name;
}

std::unique_ptr<BaseEntity> TraffikSetup::mapFields(StringMap *raw_entity)
{
}

std::list<std::string> TraffikSetup::tableViewColumns() const
{
    std::list<std::string> cols;
    cols.push_back(stationName()->displayName());
    return cols;
}

std::vector<std::string> TraffikSetup::tableViewValues()
{
    return { stationName()->displayName() };
}

QStringList TraffikSetup::tableHeaders() const
{
    return mHeader;
}

std::string TraffikSetup::searchColumn() const
{
    return stationName()->valueToString();
}

void TraffikSetup::populateEntity()
{
}

std::unique_ptr<BaseEntity> TraffikSetup::cloneAsUnique()
{
    return std::make_unique<TraffikSetup>();

}

void TraffikSetup::afterMapping(BaseEntity&)
{
}
