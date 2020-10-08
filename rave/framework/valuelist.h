#ifndef VALUELIST_H
#define VALUELIST_H

#include <QDebug>
#include "baseentity.h"

class StringField;

class ValueList : public BaseEntity
{
public:
    ValueList();
    ~ValueList() override;

    ValueList(const ValueList& rhs);
    ValueList& operator=(const ValueList& rhs);

    StringField* code() const;
    void setCode(std::string pCode);

    StringField* listValue() const;
    void setListValue(std::string pValue);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;

    virtual std::string windowTitle() const;
    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

protected:
    virtual std::string valueListTableName() const;

private:
    StringField* mCode;
    StringField* mListValue;
    QStringList mHeader;
    std::string mTableName;
    //static std::map<int, std::string> cache;
};


class Gender : public ValueList{
    public:
        Gender():ValueList(){}
        Gender(const Gender&){qDebug() << "Gender::copy ctor"; }
        std::string windowTitle() const override;
        std::string tableName() const override;

    protected:
        std::string valueListTableName() const override;

};

#endif // VALUELIST_H
