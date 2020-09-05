#ifndef ORDER_H
#define ORDER_H

#include "../framework/baseentity.h"

class StringField;
class IntegerField;
class ForeignKeyField;
class DateField;
class DateTimeField;
class Client;

template<typename T>
class ChoiceField;


class Order : public BaseEntity
{
public:
    Order();
    Order(const Client* client);
    ~Order() override;

    StringField* title()const;
    IntegerField* orderNumber()const;
    ForeignKeyField* client()const;
    DateField* orderDate()const;
    DateField* startDate()const;
    DateField* endDate()const;
    ForeignKeyField* package()const;
    ChoiceField<std::string>* revenueType()const;
    ChoiceField<std::string>* billingType()const;
    ChoiceField<std::string>* billingPeriod()const;
    ForeignKeyField* accountRep()const;
    ForeignKeyField* brand()const;
    ForeignKeyField* agency()const;
    IntegerField* spotsOrdered()const;
    IntegerField* spotsBooked()const;
    IntegerField* spotsPlayed()const;
    DecimalField* discount()const;
    DecimalField* agencyComm()const;

    ChoiceField<std::string>* agencyCommType()const;
    DecimalField* salesRepComm()const;
    ChoiceField<std::string>* salesRepCommType()const;
    DecimalField* tradeCredit()const;
    ChoiceField<std::string>* tradeCreditType()const;
    DecimalField* lateFee()const;
    IntegerField* gracePeriod()const;
    ChoiceField<std::string>* billingBasis()const;
    IntegerField* approvalCount()const;
    StringField* addLogin()const;
    DateTimeField* addDtime()const;

    void setTitle(const std::string);
    void setOrderNumber(int);
    void setClient(int);
    void setOrderDate(QDate);
    void setStartDate(QDate);
    void setEndDate(QDate);
    void setPackage(int);
    void setRevenueType(const std::string);
    void setBillingType(const std::string);
    void setBillingPeriod(const std::string);
    void setAccountRep(int);
    void setBrand(int);
    void setAgency(int);
    void setSpotsOrdered(int);
    void setSpotsBooked(int);
    void setSpotsPlayed(int);
    void setDiscount(double);
    void setAgencyComm(double);
    void setAgencyCommType(const std::string);
    void setSalesRepComm(double);
    void setSalesRepCommType(const std::string);
    void setTradeCredit(double);
    void setTradeCreditType(const std::string);
    void setLateFee(double);
    void setGracePeriod(int);
    void setBillingBasis(const std::string);
    void setApprovalCount(int);
    void setAddLogin(const std::string);
    void setAddDtime(QDateTime);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    [[nodiscard]] ActionResult validate() override;


private:
    StringField* mTitle;
    IntegerField* mOrderNumber;
    ForeignKeyField* mClient;
    DateField* mOrderDate;
    DateField* mStartDate;
    DateField* mEndDate;
    ForeignKeyField* mPackage;
    ChoiceField<std::string>* mRevenueType;
    ChoiceField<std::string>* mBillingType;
    ChoiceField<std::string>* mBillingPeriod;
    ForeignKeyField* mAccountRep;
    ForeignKeyField* mBrand;
    ForeignKeyField* mAgency;
    IntegerField* mSpotsOrdered;
    IntegerField* mSpotsBooked;
    IntegerField* mSpotsPlayed;
    DecimalField* mDiscount;
    DecimalField* mAgencyComm;

    ChoiceField<std::string>* mAgencyCommType;
    DecimalField* mSalesRepComm;
    ChoiceField<std::string>* mSalesRepCommType;
    DecimalField* mTradeCredit;
    ChoiceField<std::string>* mTradeCreditType;
    DecimalField* mLateFee;
    IntegerField* mGracePeriod;
    ChoiceField<std::string>* mBillingBasis;
    IntegerField* mApprovalCount;
    StringField* mAddLogin;
    DateTimeField* mAddDtime;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ORDER_H
