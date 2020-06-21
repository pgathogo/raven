#ifndef AGENT_H
#define AGENT_H

#include "../framework/baseentity.h"

class Agent : public BaseEntity
{
public:
    Agent();
    Agent(const Agent& other);
    ~Agent() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

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
private:
    StringField* mName;
    StringField* mTown;
    StringField* mTelephone;
    StringField* mEmail;
    StringField* mContactName;
    StringField* mMobileNo;
    TextField* mAddress;
    QStringList mHeader;
    std::string mTableName;
};

#endif // AGENT_H
