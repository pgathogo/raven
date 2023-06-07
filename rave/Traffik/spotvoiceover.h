#ifndef SPOTVOICEOVER_H
#define SPOTVOICEOVER_H

#include "../framework/manytomany.h"


class SpotVoiceOver : public ManyToMany
{
public:
    SpotVoiceOver();
    SpotVoiceOver(BaseEntity* pEnt, BaseEntity* dEnt);

    ~SpotVoiceOver() override;

    std::shared_ptr<ManyToMany> copy(BaseEntity* pEnt, BaseEntity* dEnt) const override;
    void setTable(const std::string tablename) override { setTableName(tablename); }

    IntegerField* parentId() const override;
    void setParentId(int id) override;
    IntegerField* detailId() const override;
    void setDetailId(int id) override;

    BaseEntity* parentEntity() const override;
    BaseEntity* detailEntity() const override;
    void setDetailEntity(BaseEntity* other);

    std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;
    void populateEntity() override;

    BaseEntity* mtomEntity() override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

private:
    BaseEntity* mParentEntity;
    BaseEntity* mDetailEntity;

    IntegerField* mParentId;
    IntegerField* mDetailId;

    QStringList mHeader;
    std::string mTableName;
};

#endif // SPOTVOICEOVER_H
