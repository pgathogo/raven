#ifndef TYPEEXCLUSION_H
#define TYPEEXCLUSION_H

#include<string>
#include "../framework/baseentity.h"

class Field;
class StringField;
class TextField;

class TypeExclusion : public BaseEntity
{
public:
    TypeExclusion();
    ~TypeExclusion() override;

    TypeExclusion(const TypeExclusion& other);
    TypeExclusion& operator=(const TypeExclusion& other);

    StringField* name() const;
    void setName(std::string aName);

    TextField* description() const;
    void setDescription(std::string aDescription);

    StringField* daypart1() const;
    void setDaypart1(std::string dp);

    StringField* daypart2() const;
    void setDaypart2(std::string dp);

    StringField* daypart3() const;
    void setDaypart3(std::string dp);

    StringField* daypart4() const;
    void setDaypart4(std::string dp);

    StringField* daypart5() const;
    void setDaypart5(std::string dp);

    StringField* daypart6() const;
    void setDaypart6(std::string dp);

    StringField* daypart7() const;
    void setDaypart7(std::string dp);

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
    StringField* mDaypart1;
    StringField* mDaypart2;
    StringField* mDaypart3;
    StringField* mDaypart4;
    StringField* mDaypart5;
    StringField* mDaypart6;
    StringField* mDaypart7;
    QStringList mHeader;
    std::string mTableName;
};

#endif // TYPEEXCLUSION_H
