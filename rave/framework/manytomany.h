#ifndef MANYTOMANY_H
#define MANYTOMANY_H

#include "baseentity.h"

class IntegerField;

class ManyToMany : public BaseEntity
{
public:
    ManyToMany();
    ~ManyToMany() override;

    ManyToMany(const ManyToMany& other);
    ManyToMany& operator=(const ManyToMany& other);

    IntegerField* parentId() const;
    void setParentId(int id);
    IntegerField* detailId() const;
    void setDetailId(int id);

    BaseEntity* copy() const override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

    QList<QStandardItem*> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;
    void populateEntity() override;

    virtual std::string windowTitle() const;

private:
    BaseEntity* mParentEntity;
    BaseEntity* mDetailEntity;
    IntegerField* mParentId;
    IntegerField* mDetailId;

    QStringList mHeader;
    std::string mTableName;

};

class VoiceExclusion : public ManyToMany{
    public:
        std::string windowTitle() const override;
        std::string tableName() const override;
};

#endif // MANYTOMANY_H
