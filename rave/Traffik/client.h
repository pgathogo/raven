#ifndef CLIENT_H
#define CLIENT_H

#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;

class Client : public BaseEntity //::Registrar<Client>
{
public:
    Client();
    Client(int);
    ~Client() override;

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

    StringField* name() const;
    void set_name(const std::string n);
    StringField* telephone() const;
    void set_telepone(const std::string tele);
    StringField* contact_name() const;
    void set_contact_name(const std::string contName);
    StringField* contact_mobile() const;
    void set_contact_mobile(const std::string contMobile) const;

    StringField* bill_name()const;
    StringField* address1()const;
    StringField* address2()const;
    StringField* town()const;
    StringField* postalNo()const;
    StringField* postal_code()const;
    StringField* client_email()const;
    StringField* client_mobile()const;
    ChoiceField<std::string>* contact_salute()const;
    StringField* contact_email()const;

    ForeignKeyField* agency()const;
    ForeignKeyField* group_manager()const;
    ForeignKeyField* account_manager()const;

    ChoiceField<std::string>* revenue_type()const;
    DecimalField* discount_percent()const;
    DecimalField* agency_comm()const;
    DecimalField* sale_rep_comm()const;
    DecimalField* interest_rate()const;
    DecimalField* late_fee()const;
    IntegerField* grace_period()const;
    ChoiceField<std::string>* bill_cycle()const;

    void set_bill_name(const std::string bn);
    void set_address1(const std::string address1);
    void set_address2(const std::string address2);
    void set_town(const std::string town);
    void set_postal_no(const std::string pNo);
    void set_postal_code(const std::string pCode);
    void set_client_mobile(const std::string cMobile);
    void set_client_email(const std::string cEmail);
    void set_contact_salute(const std::string cSalute);
    void set_contact_email(const std::string contEmail);
    void set_contact_mobile(const std::string contMobile);
    void set_agency(int agent);
    void set_client_group(int cGroup);
    void set_account_manager(int aManager);
    void set_revenue_type(const std::string rType);
    void set_discount_percent(double dPercent);
    void set_agency_comm(double aComm );
    void set_sale_rep_comm(double sRepComm);
    void set_interest_rate(double iRate);
    void set_late_fee(double lFee);
    void set_grace_period(int gPeriod);
    void set_bill_cycle(const std::string bCycle);

private:
    StringField* m_name;
    StringField* m_biil_name;
    StringField* m_address1;
    StringField* m_address2;
    StringField* m_town;
    StringField* m_postal_no;
    StringField* m_postal_code;
    StringField* m_telephone;
    StringField* m_client_email;
    StringField* m_client_mobile;
    StringField* m_contact_name;
    StringField* m_contact_email;
    StringField* m_contact_mobile;
    IntegerField* m_grace_period;
    DecimalField* m_discount_percent;
    DecimalField* m_agency_comm; // Commission
    DecimalField* m_sale_rep_comm;
    DecimalField* m_interest_rate;
    DecimalField* m_late_fee;

    ForeignKeyField* m_agency;
    ForeignKeyField* m_client_group;
    ForeignKeyField* m_account_manager;

    ChoiceField<std::string>* m_contact_salute;
    ChoiceField<std::string>* m_revenue_type;
    ChoiceField<std::string>* m_bill_cycle;

    QStringList mHeader;
    std::string mTableName;

};

#endif // CLIENT_H
