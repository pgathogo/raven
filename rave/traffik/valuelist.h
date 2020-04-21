#ifndef VALUELIST_H
#define VALUELIST_H

#include "../framework/baseentity.h"

class StringField;

class ValueList : public BaseEntity
{
public:
    ValueList();
    ~ValueList() override;

    ValueList(const ValueList& vl);
    ValueList& operator=(const ValueList& vl);

    StringField* code() const;
    void setCode(std::string pCode);

    StringField* listValue() const;
    void setListValue(std::string pValue);

    BaseEntity* copy() const override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    QList<QStandardItem*> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;

    virtual std::string windowTitle() const;

protected:
    virtual std::string valueListTableName() const;

private:
    StringField* mCode;
    StringField* mListValue;
    QStringList mHeader;
    std::string mTableName;
};


class Gender : public ValueList{
    public:
        std::string windowTitle() const override;
        std::string tableName() const override;
    protected:
        std::string valueListTableName() const override;

};

#endif // VALUELIST_H
