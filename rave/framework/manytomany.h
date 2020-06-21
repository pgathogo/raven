#ifndef MANYTOMANY_H
#define MANYTOMANY_H

#include "baseentity.h"


using ConstVecIter =  std::vector<std::unique_ptr<BaseEntity>>::const_iterator;

class IntegerField;

class ManyToMany : public BaseEntity
{
public:
    ManyToMany();
    ManyToMany(BaseEntity* parentEntity, BaseEntity* detailEntity);
    ~ManyToMany() override;

    ManyToMany(const ManyToMany& other);
    ManyToMany& operator=(const ManyToMany& other);

    IntegerField* parentId() const;
    void setParentId(int id);
    IntegerField* detailId() const;
    void setDetailId(int id);

    std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

    std::list<std::string> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string tableName() const override;
    void setTableName(std::string table_name) override;
    std::string searchColumn() const override;
    void populateEntity() override;

    virtual std::string windowTitle() const;
    virtual std::string typeInfo() const;

    void setParentEntity(BaseEntity* pEntity);
    void setDetailEntity(BaseEntity* dEntity);

    BaseEntity* parentEntity() const;
    BaseEntity* detailEntity() const;

    void addEntity(BaseEntity* entity);
    std::size_t getSize(){ return mEntities.size(); }

    ConstVecIter cVecBegin();
    ConstVecIter cVecEnd();

    std::vector<std::unique_ptr<BaseEntity>> const& entities() const;

protected:
    BaseEntity* mParentEntity;
    BaseEntity* mDetailEntity;

    IntegerField* mParentId;
    IntegerField* mDetailId;
private:
    std::vector<std::unique_ptr<BaseEntity>> mEntities;
    QStringList mHeader;
    std::string mTableName;

};

class VoiceExclusion : public ManyToMany{
    public:
        VoiceExclusion();
        VoiceExclusion(BaseEntity* pEnt, BaseEntity* dEnt);
        ~VoiceExclusion() override;

        std::string windowTitle() const override;
        std::string typeInfo() const override;

        std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

        std::list<std::string> tableViewColumns() override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string tableName() const override;
        void setTableName(std::string table_name) override;
        std::string searchColumn() const override;
        void populateEntity() override;

    private:
         QStringList mHeader;
         std::string mTableName;
};

#endif // MANYTOMANY_H
