#ifndef TRAFFIKSETUP_H
#define TRAFFIKSETUP_H

#include "../framework/baseentity.h"

class StringField;
class DecimalField;
class BooleanField;

template<typename T>
class ChoiceField;


class TraffikSetup : public BaseEntity
{
public:
    TraffikSetup();
    ~TraffikSetup() override;

    StringField* stationName() const;
    void setStationName(const std::string station_name);

    StringField* stationLogo() const;
    void setStationLogo(const std::string station_logo);

    StringField* address1() const;
    void setAddress1(const std::string addr1);

    StringField* address2() const;
    void setAddress2(const std::string addr2);

    DecimalField* agencyComm() const;
    void setAgencyComm(double aComm);

    ChoiceField<std::string>* agencyCommType() const;
    void setAgencyCommType(const std::string comm_type);

    DecimalField* saleRepComm() const;
    void setSaleRepComm(double rep_comm);

    ChoiceField<std::string>* saleRepCommType() const;
    void setSaleRepCommType(const std::string rep_type);

    ChoiceField<std::string>* billingCycle() const;
    void setBillingCycle(const std::string bill_cycle);

    ChoiceField<std::string>* revenueType() const;
    void setRevenueType(const std::string rev_type);

    ChoiceField<std::string>* billingType() const;
    void setBillingType(const std::string bill_type) const;

    ChoiceField<std::string>* billingBasis() const;
    void setBillingBasis(const std::string bill_basis);

    DecimalField* lateFee() const;
    void setLateFee(double late_fee);

    DecimalField* interestRate() const;
    void setInterestRate(double int_rate);

    IntegerField* gracePeriod() const;
    void setGracePeriod(int grace_period);

    IntegerField* orderApprovalLevels() const;
    void setOrderApprovalLevels(int aprv_levels);

    IntegerField* orderNumberSequence();
    void setOrderNumberSequence(int seq);

    BooleanField* orderAprvdBB() const;
    void setOrderAprvdBB(bool val) const;

    IntegerField* breakTimeInterval() const;
    void setBreakTimeInterval(int bi);

    IntegerField* breakDuration() const;
    void setBreakDuration(int bd);

    IntegerField* breakMaxSpots() const;
    void setBreakMaxSpots(int max_spots);

    StringField* audio_path() const;
    void set_audio_path(const std::string path);

    StringField* comm_audio_path() const;
    void set_comm_audio_path(const std::string path);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;
private:
    StringField* m_station_name;
    StringField* m_station_logo;
    StringField* m_address1;
    StringField* m_address2;
    DecimalField* m_agency_comm;
    ChoiceField<std::string>* m_agency_comm_type;
    DecimalField* m_sale_rep_comm;
    ChoiceField<std::string>* m_sale_rep_comm_type;
    ChoiceField<std::string>* m_billing_cycle;
    ChoiceField<std::string>* m_revenue_type;
    ChoiceField<std::string>* m_billing_type;
    ChoiceField<std::string>* m_billing_basis;
    DecimalField* m_late_fee;
    DecimalField* m_interest_rate;
    IntegerField* m_grace_period;
    IntegerField* m_order_approval_levels;
    IntegerField* m_order_number_sequence;
    BooleanField* m_order_aprvd_before_booking; // Order Approved before booking

    IntegerField* m_break_time_interval;
    IntegerField* m_break_duration;
    IntegerField* m_break_max_spots;

    StringField* m_audio_path;
    StringField* m_comm_audio_path;

    QStringList mHeader;
    std::string mTableName;
};

#endif // TRAFFIKSETUP_H
