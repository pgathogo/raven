#include "traffiksetup.h"
#include "../framework/choicefield.h"
#include "../utils/tools.h"

TraffikSetup::TraffikSetup()
{
    m_station_name = createField<StringField>("station_name", "Station Name");
    m_station_logo = createField<StringField>("station_logo","Station Logo");
    m_address1 = createField<StringField>("address1", "Address 1");
    m_address2 = createField<StringField>("address2", "Address 2");
    m_agency_comm = createField<DecimalField>("agency_comm", "Agency Commision");

    m_agency_comm_type = createField<ChoiceField<std::string>>("agency_comm_type", "Agency Commision Type");
    m_agency_comm_type->addChoice({"P", "Percentage"});
    m_agency_comm_type->addChoice({"F", "Fixed"});

    m_sale_rep_comm = createField<DecimalField>("sales_rep_comm", "Sale Rep Commission");

    m_sale_rep_comm_type = createField<ChoiceField<std::string>>("sales_rep_comm_type", "Sales Commission Type");
    m_sale_rep_comm_type->addChoice({"P", "Percentage"});
    m_sale_rep_comm_type->addChoice({"F", "Fixed"});

    m_billing_cycle = createField<ChoiceField<std::string>>("billing_cycle", "Billing Cycle");
    m_billing_cycle->addChoice({"W", "Weekly"});
    m_billing_cycle->addChoice({"B", "Bi-Weekly"});
    m_billing_cycle->addChoice({"M", "Monthly"});

    m_revenue_type = createField<ChoiceField<std::string>>("revenue_type", "Revenue Type");
    m_revenue_type->addChoice({"C", "Cash"});
    m_revenue_type->addChoice({"T", "Trade"});

    m_billing_type = createField<ChoiceField<std::string>>("billing_type", "Billing Type");
    m_billing_type->addChoice({"G", "Gross"});
    m_billing_type->addChoice({"N", "Net"});

    m_billing_basis = createField<ChoiceField<std::string>>("billing_basis", "Billing Basis");
    m_billing_basis->addChoice({"STD", "Standard"});
    m_billing_basis->addChoice({"DAY", "Daily"});
    m_billing_basis->addChoice({"PRD", "Billing Period"});

    m_late_fee = createField<DecimalField>("late_fee", "Late Fee");
    m_interest_rate = createField<DecimalField>("interest_rate", "Interest Rate");
    m_grace_period = createField<IntegerField>("pay_grace_period", "Grace Period");
    m_order_approval_levels = createField<IntegerField>("order_approval_levels", "Approval Levels");
    m_order_number_sequence = createField<IntegerField>("order_number_sequence", "Order Num Sequence");

    m_order_aprvd_before_booking = createField<BooleanField>("order_approved_before_booking", "Approve Before Booking");

    m_break_time_interval = createField<IntegerField>("break_time_interval", "Break Interval");
    m_break_duration = createField<IntegerField>("break_duration", "Break Duration");
    m_break_max_spots = createField<IntegerField>("break_max_spots", "Break Max Spots");

    m_audio_path = createField<StringField>("audio_path", "Audio Path");
    m_comm_audio_path = createField<StringField>("comm_audio_path", "Comm Audio Path");

    mHeader << stoq(m_station_name->fieldName());
    setTableName("rave_setup");
}

TraffikSetup::~TraffikSetup()
{
}

StringField *TraffikSetup::stationName() const
{
    return m_station_name;
}

void TraffikSetup::setStationName(const std::string station_name)
{
    m_station_name->setValue( station_name );
}

StringField *TraffikSetup::stationLogo() const
{
    return m_station_logo;
}

void TraffikSetup::setStationLogo(const std::string station_logo)
{
    m_station_logo->setValue( station_logo );
}

StringField *TraffikSetup::address1() const
{
    return m_address1;
}

void TraffikSetup::setAddress1(const std::string addr1)
{
    m_address1->setValue( addr1 );
}

StringField *TraffikSetup::address2() const
{
    return m_address2;
}

void TraffikSetup::setAddress2(const std::string addr2)
{
    m_address2->setValue( addr2 );
}

DecimalField *TraffikSetup::agencyComm() const
{
    return m_agency_comm;
}

void TraffikSetup::setAgencyComm(double aComm)
{
    m_agency_comm->setValue( aComm );
}

ChoiceField<std::string> *TraffikSetup::agencyCommType() const
{
    return m_agency_comm_type;
}

void TraffikSetup::setAgencyCommType(const std::string comm_type)
{
    m_agency_comm_type->setValue( comm_type );
}

DecimalField *TraffikSetup::saleRepComm() const
{
    return m_sale_rep_comm;
}

void TraffikSetup::setSaleRepComm(double rep_comm)
{
    m_sale_rep_comm->setValue( rep_comm );

}

ChoiceField<std::string> *TraffikSetup::saleRepCommType() const
{
    return m_sale_rep_comm_type;
}

void TraffikSetup::setSaleRepCommType(const std::string rep_type)
{
    m_sale_rep_comm_type->setValue( rep_type );
}

ChoiceField<std::string> *TraffikSetup::billingCycle() const
{
    return m_billing_cycle;
}

void TraffikSetup::setBillingCycle(const std::string bill_cycle)
{
    m_billing_cycle->setValue( bill_cycle );
}

ChoiceField<std::string> *TraffikSetup::revenueType() const
{
    return m_revenue_type;
}

void TraffikSetup::setRevenueType(const std::string rev_type)
{
    m_revenue_type->setValue( rev_type );
}

ChoiceField<std::string> *TraffikSetup::billingType() const
{
    return m_billing_type;
}

void TraffikSetup::setBillingType(const std::string bill_type) const
{
    m_billing_type->setValue( bill_type );
}

ChoiceField<std::string> *TraffikSetup::billingBasis() const
{
    return m_billing_basis;
}

void TraffikSetup::setBillingBasis(const std::string bill_basis)
{
    m_billing_basis->setValue( bill_basis );
}

DecimalField *TraffikSetup::lateFee() const
{
    return m_late_fee;
}

void TraffikSetup::setLateFee(double late_fee)
{
    m_late_fee->setValue( late_fee );
}

DecimalField *TraffikSetup::interestRate() const
{
    return m_interest_rate;
}

void TraffikSetup::setInterestRate(double int_rate)
{
    m_interest_rate->setValue( int_rate );
}

IntegerField *TraffikSetup::gracePeriod() const
{
    return m_grace_period;
}

void TraffikSetup::setGracePeriod(int grace_period)
{
    m_grace_period->setValue( grace_period );
}

IntegerField *TraffikSetup::orderApprovalLevels() const
{
    return m_order_approval_levels;
}

void TraffikSetup::setOrderApprovalLevels(int aprv_levels)
{
    m_order_approval_levels->setValue(aprv_levels);
}

IntegerField *TraffikSetup::orderNumberSequence()
{
    return m_order_number_sequence;
}

void TraffikSetup::setOrderNumberSequence(int seq)
{
    m_order_number_sequence->setValue(seq);
}

BooleanField *TraffikSetup::orderAprvdBB() const
{
    return m_order_aprvd_before_booking;

}

void TraffikSetup::setOrderAprvdBB(bool val) const
{
    m_order_aprvd_before_booking->setValue( val );
}

IntegerField *TraffikSetup::breakTimeInterval() const
{
    return m_break_time_interval;
}

void TraffikSetup::setBreakTimeInterval(int bi)
{
    m_break_time_interval->setValue( bi );
}

IntegerField *TraffikSetup::breakDuration() const
{
    return m_break_duration;
}

void TraffikSetup::setBreakDuration(int bd)
{
    m_break_duration->setValue( bd );
}

IntegerField *TraffikSetup::breakMaxSpots() const
{
    return m_break_max_spots;
}

void TraffikSetup::setBreakMaxSpots(int max_spots)
{
    m_break_max_spots->setValue( max_spots );
}

StringField *TraffikSetup::audio_folder() const
{
    return m_audio_path;
}

void TraffikSetup::set_audio_folder(const std::string path)
{
    m_audio_path->setValue(path);
}

StringField *TraffikSetup::comm_audio_folder() const
{
    return m_comm_audio_path;
}

void TraffikSetup::set_comm_audio_folder(const std::string path)
{
    m_comm_audio_path->setValue(path);
}

std::string TraffikSetup::tableName() const
{
    return mTableName;
}

void TraffikSetup::setTableName(const std::string table_name)
{
   mTableName = table_name;
}

std::unique_ptr<BaseEntity> TraffikSetup::mapFields(StringMap*)
{
}

std::vector<std::string> TraffikSetup::tableViewColumns() const
{
    return tableViewCols<std::string>(stationName()->displayName());

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
