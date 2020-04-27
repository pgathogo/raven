#include "manytomany.h"

ManyToMany::ManyToMany()
    :mParentEntity{}
    ,mDetailEntity{}
    ,mParentId{}
    ,mDetailId{}
{
}

ManyToMany::~ManyToMany()
{
    delete mParentEntity;
    delete mDetailEntity;

    delete mParentId;
    delete mDetailId;
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

/* ---------------- VoiceExclusion -----------------*/
std::string VoiceExclusion::windowTitle() const
{
    return "Voice Exclusions";
}

std::string VoiceExclusion::tableName() const
{
    return "rave_voiceexclusion";
}
