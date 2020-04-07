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
    BaseEntity* copy() const override;
    BaseEntity* mapFields(StringMap* e) override;

    StringField* name() const;
    void setName(std::string aName);

    TextField* description() const;
    void setDescription(std::string aDescription);

    QList<QStandardItem*> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    std::string searchColumn() const override;

private:
    StringField* mName;
    TextField* mDescription;
    QStringList mHeader;
};

#endif // CLIENTGROUP_H
