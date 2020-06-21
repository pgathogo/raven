#ifndef CLIENT_H
#define CLIENT_H

#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;

class Client : public BaseEntity
{
public:
    Client();
    ~Client() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    StringField* name() const;
    void setName(const std::string n);
    StringField* telephone() const;
    void setTelephone(const std::string tele);
    StringField* contactName() const;
    void setContactName(const std::string contName);
    StringField* contactMobile() const;
    void setContactMobile(const std::string contMobile) const;

    StringField* billName()const;
    StringField* address1()const;
    StringField* address2()const;
    StringField* town()const;
    StringField* postalNo()const;
    StringField* postalCode()const;
    StringField* clientEmail()const;
    StringField* clientMobile()const;
    ChoiceField<std::string>* contactSalute()const;
    StringField* contactEmail()const;

    ForeignKeyField* agency()const;
    ForeignKeyField* clientGroup()const;
    ForeignKeyField* accountManager()const;

    ChoiceField<std::string>* revenueType()const;
    DecimalField* discountPercent()const;
    DecimalField* agencyComm()const;
    DecimalField* saleRepComm()const;
    DecimalField* interestRate()const;
    DecimalField* lateFee()const;
    IntegerField* gracePeriod()const;
    ChoiceField<std::string>* billCycle()const;

    void setBillName(const std::string bn);
    void setAddress1(const std::string address1);
    void setAddress2(const std::string address2);
    void setTown(const std::string town);
    void setPostalNo(const std::string pNo);
    void setPostalCode(const std::string pCode);
    void setClientMobile(const std::string cMobile);
    void setClientEmail(const std::string cEmail);
    void setContactSalute(const std::string cSalute);
    void setContactEmail(const std::string contEmail);
    void setContactMobile(const std::string contMobile);
    void setAgency(int agent);
    void setClientGroup(int cGroup);
    void setAccountManager(int aManager);
    void setRevenueType(const std::string rType);
    void setDiscountPercent(double dPercent);
    void setAgencyComm(double aComm );
    void setSaleRepComm(double sRepComm);
    void setInterestRate(double iRate);
    void setLateFee(double lFee);
    void setGracePeriod(int gPeriod);
    void setBillCycle(const std::string bCycle);

private:
    StringField* mName;
    StringField* mBillName;
    StringField* mAddress1;
    StringField* mAddress2;
    StringField* mTown;
    StringField* mPostalNo;
    StringField* mPostalCode;
    StringField* mTelephone;
    StringField* mClientEmail;
    StringField* mClientMobile;
    StringField* mContactName;
    StringField* mContactEmail;
    StringField* mContactMobile;
    IntegerField* mGracePeriod;
    DecimalField* mDiscountPercent;
    DecimalField* mAgencyComm; // Commission
    DecimalField* mSaleRepComm;
    DecimalField* mInterestRate;
    DecimalField* mLateFee;

    ForeignKeyField* mAgency;
    ForeignKeyField* mClientGroup;
    ForeignKeyField* mAccountManager;

    ChoiceField<std::string>* mContactSalute;
    ChoiceField<std::string>* mRevenueType;
    ChoiceField<std::string>* mBillCycle;
    QStringList mHeader;
    std::string mTableName;

};

#endif // CLIENT_H
