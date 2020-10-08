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
    StringField* mStationName;
    StringField* mStationLogo;
    StringField* mAddress1;
    StringField* mAddress2;
    DecimalField* mAgencyComm;
    ChoiceField<std::string>* mAgencyCommType;
    DecimalField* mSaleRepComm;
    ChoiceField<std::string>* mSaleRepCommType;
    ChoiceField<std::string>* mBillingCycle;
    ChoiceField<std::string>* mRevenueType;
    ChoiceField<std::string>* mBillingType;
    ChoiceField<std::string>* mBillingBasis;
    DecimalField* mLateFee;
    DecimalField* mInterestRate;
    IntegerField* mGracePeriod;
    IntegerField* mOrderApprovalLevels;
    IntegerField* mOrderNumberSequence;
    BooleanField* mOrderAprvdBB; // Order Approved before booking

    IntegerField* mBreakTimeInterval;
    IntegerField* mBreakDuration;
    IntegerField* mBreakMaxSpots;

    QStringList mHeader;
    std::string mTableName;
};

#endif // TRAFFIKSETUP_H
