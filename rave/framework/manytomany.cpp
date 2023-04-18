#include <QDebug>
#include <QStandardItem>
#include "manytomany.h"
#include "typeexclusion.h"

ManyToMany::~ManyToMany()
{
}
void ManyToMany::addEntity(BaseEntity* entity)
{
    mEntities.emplace_back(std::move(entity));
}

ConstVecIter ManyToMany::cVecBegin()
{
    return mEntities.cbegin();
}
ConstVecIter ManyToMany::cVecEnd()
{
    return mEntities.cend();
}

std::vector<std::unique_ptr<BaseEntity>> const& ManyToMany::entities() const
{
    return mEntities;
}

/*
void ManyToMany::print_selected_members(const QStringList &members) const
{
    QStringList::const_iterator const_iter;
    for(const_iter=members.constBegin(); const_iter != members.constEnd(); ++const_iter){
        for(auto& [key, field] : fields()){
            if (key == (*const_iter).toLocal8Bit().constData()){
                print_member(key, field->valueToString());
            }
        }
    }

}

void ManyToMany::print_all_members() const
{
    for(auto& [key, field] : fields()) {
        print_member(key, field->valueToString());
    }
}

void ManyToMany::print_member(const std::string member, const std::string value) const
{
    qDebug() << stoq(member) << "=" << stoq(value);
}
*/

std::string ManyToMany::typeInfo() const
{
}
/*
void ManyToMany::print_members(const QStringList &members) const
{
    if (members.size() > 0){
        print_selected_members(members);
    }else{
        print_all_members();
    }

    printstr("--- END ---");
}
*/

/* ---------------- VoiceExclusion -----------------*/
VoiceExclusion::VoiceExclusion()
    :ManyToMany{}{}

VoiceExclusion::VoiceExclusion(BaseEntity* pEnt, BaseEntity* dEnt)
    :ManyToMany{}
    ,mParentEntity{pEnt}
    ,mDetailEntity{dEnt}

{
    mParentId = createField<IntegerField>("parent_id","Parent ID");
    mDetailId = createField<IntegerField>("detail_id","Detail ID");

    mHeader = dEnt->tableHeaders();
    setTableName("rave_voiceexclusion");
}

VoiceExclusion::VoiceExclusion(const VoiceExclusion& other)
{
    if (other.mParentEntity)
        mParentEntity = other.mParentEntity;
    if (other.mDetailEntity)
        mDetailEntity = other.mDetailEntity;
    if (other.mParentId)
        mParentId = other.mParentId;
    if (other.mDetailId)
        mDetailId = other.mDetailId;
}

VoiceExclusion& VoiceExclusion::operator=(const VoiceExclusion& other)
{
    if (this != &other){
        delete mParentEntity;
        delete mDetailEntity;
        delete mParentId;
        delete mDetailId;

        mParentEntity = other.mParentEntity;
        mDetailEntity = other.mDetailEntity;

        mParentId = other.mParentId;
        mDetailId = other.mDetailId;
    }

    return *this;
}

VoiceExclusion::~VoiceExclusion()
{
}

std::unique_ptr<ManyToMany> VoiceExclusion::copy(BaseEntity* pEnt, BaseEntity* dEnt) const
{
    return std::make_unique<VoiceExclusion>(pEnt, dEnt);
}

std::string VoiceExclusion::tableName() const
{
    return "rave_voiceexclusion";
}

std::string VoiceExclusion::typeInfo() const
{
    return "voice_type_exclusion";
}

std::unique_ptr<BaseEntity> VoiceExclusion::mapFields(StringMap* sm)
{
    auto ve = entityFieldMap<VoiceExclusion>(sm,
                                             std::move(mParentEntity),
                                             std::move(mDetailEntity));

   // getEntityById(*mDetailEntity, ve->detailId()->value());

    return std::move(ve);
}

void VoiceExclusion::setDetailEntity(BaseEntity* other)
{
    mDetailEntity = other;
}

void VoiceExclusion::afterMapping(BaseEntity& entity)
{
    VoiceExclusion& ve = dynamic_cast<VoiceExclusion&>(entity);
    auto te = ve.detailEntity()->cloneAsShared();
    ve.setDetailEntity(te.get());
    getEntityById(std::move(te), ve.detailId()->value());
}

std::vector<std::string> VoiceExclusion::tableViewColumns() const
{
    TRAFFIK::TypeExclusion* te = dynamic_cast<TRAFFIK::TypeExclusion*>(mDetailEntity);

    return tableViewCols<std::string>(
                te->name()->displayName(),
                te->description()->displayName()
                );

}
std::vector<std::string> VoiceExclusion::tableViewValues()
{
    TRAFFIK::TypeExclusion* te = dynamic_cast<TRAFFIK::TypeExclusion*>(mDetailEntity);
    return tableViewCols<std::string>(
                te->name()->valueToString(),
                te->description()->valueToString()
                );
}
QStringList VoiceExclusion::tableHeaders() const
{
    return mHeader;
}

void VoiceExclusion::setTableName(std::string table_name)
{
    mTableName = table_name;
}
std::string VoiceExclusion::searchColumn() const
{
    return mDetailEntity->searchColumn();
}

void VoiceExclusion::populateEntity()
{
    mParentId->setValue(mParentEntity->id());
    mDetailId->setValue(mDetailEntity->id());
}

IntegerField* VoiceExclusion::parentId() const
{
    return mParentId;
}

void VoiceExclusion::setParentId(int id)
{
    mParentId->setValue(id);
}

IntegerField* VoiceExclusion::detailId() const
{
    return mDetailId;
}

void VoiceExclusion::setDetailId(int id)
{
    mDetailId->setValue(id);
}

BaseEntity* VoiceExclusion::parentEntity() const
{
    return mParentEntity;
}

BaseEntity* VoiceExclusion::detailEntity() const
{
    return mDetailEntity;
}

BaseEntity* VoiceExclusion::mtomEntity()
{
    return this;
}

std::shared_ptr<BaseEntity> VoiceExclusion::cloneAsShared()
{
    return std::make_shared<VoiceExclusion>(mParentEntity, mDetailEntity);
}

