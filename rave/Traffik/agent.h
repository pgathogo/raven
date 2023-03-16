#ifndef AGENT_H
#define AGENT_H

#include "../framework/baseentity.h"

class Agent : public BaseEntity //::Registrar<Agent>
{
public:
    Agent();
    Agent(int);
    Agent(const Agent& other);
    ~Agent() override;

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
    void setName(std::string pname);

    StringField* town() const;
    void setTown(std::string ptown);

    StringField* telephone() const;
    void setTelephone(std::string ptele);

    StringField* email() const;
    void setEmail(std::string pemail);

    StringField* contactName() const;
    void setContactName(std::string pcontact_name);

    StringField* mobileNo() const;
    void setMobileNo(std::string pmobileno);

    TextField* address() const;
    void setAddress(std::string paddress);

    //std::string className() override;
private:
    StringField* m_name;
    StringField* m_town;
    StringField* m_telephone;
    StringField* m_email;
    StringField* mContactName;
    StringField* mMobileNo;
    TextField* mAddress;
    QStringList mHeader;
    std::string mTableName;
};

#endif // AGENT_H
