#ifndef CONTENTAUTH_H
#define CONTENTAUTH_H

#include "../framework/baseentity.h"

using Bits = std::tuple<int, int, int, int>;


class ContentAuth : public BaseEntity
{
public:
    ContentAuth();
    ~ContentAuth() override;

    ForeignKeyField* content() const;
    void setContent(int val);

    ForeignKeyField* role() const;
    void setRole(int val);

    StringField* accessBit() const;
    void setAccessBit(std::string abit);

    IntegerField* createBit() const;
    void setCreateBit(int bit);

    IntegerField* readBit() const;
    void setReadBit(int bit);

    IntegerField* updateBit() const;
    void setUpdateBit(int bit);

    IntegerField* deleteBit() const;
    void setDeleteBit(int bit);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

    Bits getAccessBit(std::string abit);
    std::string access_bit_tostring();

private:
    ForeignKeyField* mContent;
    ForeignKeyField* mRole;
    StringField* mAccessBit;
    IntegerField* mCreateBit;
    IntegerField* mReadBit;
    IntegerField* mUpdateBit;
    IntegerField* mDeleteBit;

    QStringList mHeader;
    std::string mTableName;
};

#endif // CONTENTAUTH_H
