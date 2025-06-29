#ifndef CLIENTGROUP_H
#define CLIENTGROUP_H

#include <string>
#include "../../../rave/framework/baseentity.h"

#include <QDebug>

class Field;
class IntegerField;
class TextField;

class ClientGroup : public BaseEntity //::Registrar<ClientGroup>
{
public:
    ClientGroup();
    ClientGroup(int);
    ~ClientGroup() override;

    ClientGroup(const ClientGroup& other);
    ClientGroup& operator=(const ClientGroup& other);

    StringField* name() const;
    void setName(std::string aName);

    TextField* description() const;
    void setDescription(std::string aDescription);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;
    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

//    std::string className() override;


private:
    StringField* mName;
    TextField* mDescription;
    QStringList mHeader;
    std::string mTableName;
};

#endif // CLIENTGROUP_H
