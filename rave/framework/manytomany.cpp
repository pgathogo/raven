#include <QDebug>
#include <QStandardItem>
#include "manytomany.h"
#include "typeexclusion.h"

ManyToMany::ManyToMany()
    :mParentEntity{}
    ,mDetailEntity{}
    ,mParentId{}
    ,mDetailId{}
{}

ManyToMany::ManyToMany(BaseEntity* parentEntity, BaseEntity* detailEntity)
    :mParentEntity{parentEntity}
    ,mDetailEntity{detailEntity}
    ,mParentId{}
    ,mDetailId{}
    ,mEntities{}
{
    mParentId = createField<IntegerField>("parent_id","Parent ID");
    mDetailId = createField<IntegerField>("detail_id","Detail ID");
}

ManyToMany::~ManyToMany()
{
    qDebug() << "ManyToMany::dtor" ;
    delete mParentEntity;
    delete mDetailEntity;
}

ManyToMany::ManyToMany(const ManyToMany& other)
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

ManyToMany& ManyToMany::operator=(const ManyToMany& other)
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

IntegerField* ManyToMany::parentId() const
{
    return mParentId;
}
void ManyToMany::setParentId(int id)
{
    mParentId->setValue(id);
}
IntegerField* ManyToMany::detailId() const
{
    return mDetailId;
}
void ManyToMany::setDetailId(int id)
{
    mDetailId->setValue(id);
}

BaseEntity* ManyToMany::copy() const
{
    return new ManyToMany();
}
std::unique_ptr<BaseEntity> ManyToMany::mapFields(StringMap* sm)
{
    auto te = entityFieldMap<ManyToMany>(sm);
    return std::move(te);
}

QList<QStandardItem*> ManyToMany::tableViewColumns()
{
    return mDetailEntity->tableViewColumns();
}
std::vector<std::string> ManyToMany::tableViewValues()
{
    std::string pid = mParentId->valueToString();
    std::string did = mDetailId->valueToString();
    return {pid, did};
}
QStringList ManyToMany::tableHeaders() const
{
    return mHeader;
}
std::string ManyToMany::tableName() const
{
    return mTableName;
}

void ManyToMany::setTableName(std::string table_name)
{
}
std::string ManyToMany::searchColumn() const
{
    return mDetailEntity->searchColumn();
}

void ManyToMany::populateEntity()
{
    mParentId->setValue(mParentEntity->id());
    mDetailId->setValue(mDetailEntity->id());
}

std::string ManyToMany::windowTitle() const
{
    return " ";
}

void ManyToMany::setParentEntity(BaseEntity* pEntity)
{
    mParentEntity = pEntity;
}

void ManyToMany::setDetailEntity(BaseEntity* dEntity)
{
    mDetailEntity = dEntity;
}

BaseEntity* ManyToMany::parentEntity() const
{
    return mParentEntity;
}
BaseEntity* ManyToMany::detailEntity() const
{
    return mDetailEntity;
}

std::string ManyToMany::typeInfo() const
{
    return "base_type";
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

/* ---------------- VoiceExclusion -----------------*/
VoiceExclusion::VoiceExclusion()
    :ManyToMany{}{}

VoiceExclusion::VoiceExclusion(BaseEntity* pEnt, BaseEntity* dEnt):
    ManyToMany{pEnt, dEnt}
{
    TypeExclusion* te = dynamic_cast<TypeExclusion*>(dEnt);
    mHeader = te->tableHeaders();
    setTableName("rave_typeexclusion");
}

VoiceExclusion::~VoiceExclusion()
{
    qDebug() << "VoiceExxclusion::dtor";
}

std::string VoiceExclusion::windowTitle() const
{
    return "Voice Exclusions";
}

std::string VoiceExclusion::tableName() const
{
    return "rave_voiceexclusion";
}

std::string VoiceExclusion::typeInfo() const
{
    return "voice_type_exclusion";
}

BaseEntity* VoiceExclusion::copy() const
{
    return new VoiceExclusion();
}
std::unique_ptr<BaseEntity> VoiceExclusion::mapFields(StringMap* sm)
{
    auto ve = entityFieldMap<VoiceExclusion>(sm,
                                             mParentEntity,
                                             mDetailEntity);

    getEntityById(*mDetailEntity, ve->detailId()->value().toInt());

    return std::move(ve);
}

QList<QStandardItem*> VoiceExclusion::tableViewColumns()
{
    TypeExclusion* te = dynamic_cast<TypeExclusion*>(mDetailEntity);
    QString gname  = QString::fromStdString(te->name()->displayName());
    QString desc = QString::fromStdString(te->description()->displayName());

    QStandardItem* Qgname = new QStandardItem(gname);
    QStandardItem* Qdesc = new QStandardItem(desc);
    return{Qgname, Qdesc};
}
std::vector<std::string> VoiceExclusion::tableViewValues()
{
    TypeExclusion* te = dynamic_cast<TypeExclusion*>(mDetailEntity);
    std::string pname = te->name()->valueToString();
    std::string pdesc = te->description()->valueToString();
    return {pname, pdesc};
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
