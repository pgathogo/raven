#ifndef CONTENT_H
#define CONTENT_H

#include "../framework/baseentity.h"

class StringField;


class Content : public BaseEntity
{
public:
    Content();
    ~Content() override;

    StringField* name() const;
    void setName(const std::string val);

    StringField* contentDisplayName() const;
    void setDisplayName(const std::string val);

    StringField* code() const;
    void setCode(const std::string val);

    StringField* contentTableName() const;
    void setContentTableName(const std::string table_name);

    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::list<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::string searchColumn() const override;

    void populateEntity() override;
    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

private:
    StringField* mName;
    StringField* mContentDisplayName;
    StringField* mCode;
    StringField* mContentTableName;
    QStringList mHeader;
    std::string mTableName;
};

#endif // CONTENT_H
