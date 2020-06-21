#ifndef CLIENTGROUP_H
#define CLIENTGROUP_H

#include <string>
#include "../framework/baseentity.h"

#include <QDebug>

class Field;
class IntegerField;
class TextField;

class ClientGroup : public BaseEntity
{
public:
    ClientGroup();
    ~ClientGroup() override;

    ClientGroup(const ClientGroup& other);
    ClientGroup& operator=(const ClientGroup& other);

    StringField* name() const;
    void setName(std::string aName);

    TextField* description() const;
    void setDescription(std::string aDescription);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::list<std::string> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;

private:
    StringField* mName;
    TextField* mDescription;
    QStringList mHeader;
    std::string mTableName;
};

#endif // CLIENTGROUP_H
