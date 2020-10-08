#ifndef MANYTOMANY_H
#define MANYTOMANY_H

#include "baseentity.h"


using ConstVecIter =  std::vector<std::unique_ptr<BaseEntity>>::const_iterator;

class IntegerField;

class ManyToMany : public BaseEntity
{
public:
    ManyToMany(){}
    virtual ~ManyToMany();

    virtual std::unique_ptr<ManyToMany> copy(BaseEntity* pEnt, BaseEntity* dEnt) const = 0;

    virtual IntegerField* parentId() const = 0;
    virtual void setParentId(int id) = 0;
    virtual IntegerField* detailId() const = 0;
    virtual void setDetailId(int id) = 0;

    virtual std::string typeInfo() const;

    virtual BaseEntity* parentEntity() const = 0;
    virtual BaseEntity* detailEntity() const = 0;

    virtual BaseEntity* mtomEntity() = 0;

    void addEntity(BaseEntity* entity);
    std::size_t getSize(){ return mEntities.size(); }

    ConstVecIter cVecBegin();
    ConstVecIter cVecEnd();

    virtual void setTable(const std::string t) = 0;
    void setT(const std::string tabname){ this->setTable(tabname); }

    std::vector<std::unique_ptr<BaseEntity>> const& entities() const;

private:
    std::vector<std::unique_ptr<BaseEntity>> mEntities;
};



class VoiceExclusion : public ManyToMany{
    public:
        VoiceExclusion();
        VoiceExclusion(BaseEntity* pEnt, BaseEntity* dEnt);

        VoiceExclusion(const VoiceExclusion& other);
        VoiceExclusion& operator=(const VoiceExclusion& other);

        ~VoiceExclusion() override;

        std::unique_ptr<ManyToMany> copy(BaseEntity* pEnt, BaseEntity* dEnt) const override;
        void setTable(const std::string tablename) override { setTableName(tablename); }

        IntegerField* parentId() const override;
        void setParentId(int id) override;
        IntegerField* detailId() const override;
        void setDetailId(int id) override;

        BaseEntity* parentEntity() const override;
        BaseEntity* detailEntity() const override;
        void setDetailEntity(BaseEntity* other);

        //std::string windowTitle() const override;

        std::string typeInfo() const override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* sm) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string tableName() const override;
        void setTableName(std::string table_name) override;
        std::string searchColumn() const override;
        void populateEntity() override;

        BaseEntity* mtomEntity() override;

        std::unique_ptr<BaseEntity> cloneAsUnique() override;
        void afterMapping(BaseEntity& entity) override;

    private:
        BaseEntity* mParentEntity;
        BaseEntity* mDetailEntity;

        IntegerField* mParentId;
        IntegerField* mDetailId;

        QStringList mHeader;
        std::string mTableName;
};

#endif // MANYTOMANY_H
