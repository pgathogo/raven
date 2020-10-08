#include "content.h"
#include "role.h"
#include "contentauth.h"

ContentAuth::ContentAuth()
{
   mContent = createField<ForeignKeyField>("parent_id", "Content",
                                           std::make_unique<Content>(),"name");
   mRole = createField<ForeignKeyField>("detail_id", "Role",
                                        std::make_unique<Role>(), "rolname",
                                        "rolcanlogin=false");
   mAccessBit = createField<StringField>("access_bit", "Access Bit");

   mCreateBit = createField<IntegerField>("Create", "Create");
   mCreateBit->setFormOnly(true);
   mCreateBit->setSearchable(false);
   setCreateBit(0);

   mReadBit = createField<IntegerField>("Read", "Read");
   mReadBit->setFormOnly(true);
   mReadBit->setSearchable(false);
   setReadBit(0);

   mUpdateBit = createField<IntegerField>("Update", "Update");
   mUpdateBit->setFormOnly(true);
   mUpdateBit->setSearchable(false);
   setUpdateBit(0);

   mDeleteBit = createField<IntegerField>("Delete", "Delete");
   mDeleteBit->setFormOnly(true);
   mDeleteBit->setSearchable(false);
   setDeleteBit(0);

   mHeader << QString::fromStdString(mContent->fieldLabel())
           << QString::fromStdString(mRole->fieldLabel())
           << QString::fromStdString(mCreateBit->fieldLabel())
           << QString::fromStdString(mReadBit->fieldLabel())
           << QString::fromStdString(mUpdateBit->fieldLabel())
           << QString::fromStdString(mDeleteBit->fieldLabel());

   setTableName("rave_contentauth");
}

ContentAuth::~ContentAuth()
{
}

ForeignKeyField *ContentAuth::content() const
{
    return mContent;
}

void ContentAuth::setContent(int val)
{
    mContent->setValue( val );
}

ForeignKeyField *ContentAuth::role() const
{
    return mRole;
}

void ContentAuth::setRole(int val)
{
    mRole->setValue( val );
}

StringField *ContentAuth::accessBit() const
{
    return mAccessBit;
}

void ContentAuth::setAccessBit(std::string abit)
{
    mAccessBit->setValue( abit );
}

IntegerField *ContentAuth::createBit() const
{
    return mCreateBit;
}

void ContentAuth::setCreateBit(int bit)
{
    mCreateBit->setValue( bit );
}

IntegerField *ContentAuth::readBit() const
{
    return mReadBit;
}

void ContentAuth::setReadBit(int bit)
{
    mReadBit->setValue( bit );
}

IntegerField *ContentAuth::updateBit() const
{
    return mUpdateBit;
}

void ContentAuth::setUpdateBit(int bit)
{
    mUpdateBit->setValue( bit );
}

IntegerField *ContentAuth::deleteBit() const
{
    return mDeleteBit;
}

void ContentAuth::setDeleteBit(int bit)
{
    mDeleteBit->setValue( bit );
}

std::string ContentAuth::tableName() const
{
    return mTableName;
}

void ContentAuth::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> ContentAuth::mapFields(StringMap *raw_entity)
{
}

std::vector<std::string> ContentAuth::tableViewColumns() const
{
    return tableViewCols<std::string>(
                content()->displayName(),
                role()->displayName(),
                createBit()->displayName(),
                readBit()->displayName(),
                updateBit()->displayName(),
                deleteBit()->displayName()
                );
}

std::vector<std::string> ContentAuth::tableViewValues()
{
    return tableViewColumns();
}

QStringList ContentAuth::tableHeaders() const
{
    return mHeader;
}

std::string ContentAuth::searchColumn() const
{
    return content()->valueToString();
}

void ContentAuth::populateEntity()
{
}

std::unique_ptr<BaseEntity> ContentAuth::cloneAsUnique()
{
    return std::make_unique<ContentAuth>();
}

void ContentAuth::afterMapping(BaseEntity& entity)
{
    ContentAuth& cauth = dynamic_cast<ContentAuth&>(entity);
    auto[c, r, u, d] = getAccessBit(cauth.accessBit()->value());
    setCreateBit(c);
    setReadBit(r);
    setUpdateBit(u);
    setDeleteBit(d);
}

Bits ContentAuth::getAccessBit(std::string abit)
{
   std::string c{abit.at(0)};
   std::string r{abit.at(1)};
   std::string u{abit.at(2)};
   std::string d{abit.at(3)};

   Bits b = std::make_tuple(
               std::atoi(c.c_str()),
               std::atoi(r.c_str()),
               std::atoi(u.c_str()),
               std::atoi(d.c_str())
               );
   return b;
}

std::string ContentAuth::access_bit_tostring()
{
    return std::to_string(createBit()->value())+
            std::to_string(readBit()->value())+
            std::to_string(updateBit()->value())+
            std::to_string(deleteBit()->value());
}
