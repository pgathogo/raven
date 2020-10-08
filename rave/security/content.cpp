#include "content.h"

Content::Content()
{
    mName = createField<StringField>("name","Name");
    mContentDisplayName = createField<StringField>("display_name", "Display Name");
    mCode = createField<StringField>("code", "Code");
    mContentTableName = createField<StringField>("table_name", "Table Name");

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mContentDisplayName->fieldLabel())
            << QString::fromStdString(mContentTableName->fieldLabel());

    setTableName("rave_content");
}

Content::~Content()
{

}

StringField *Content::name() const
{
    return mName;
}

void Content::setName(const std::string val)
{
    mName->setValue( val );
}

StringField *Content::contentDisplayName() const
{
    return mContentDisplayName;
}

void Content::setDisplayName(const std::string val)
{
    mContentDisplayName->setValue( val );
}

StringField *Content::code() const
{
    return mCode;
}

void Content::setCode(const std::string val)
{
    mCode->setValue( val );
}

StringField *Content::contentTableName() const
{
    return mContentTableName;
}

void Content::setContentTableName(const std::string table_name)
{
    mContentTableName->setValue( table_name );
}

std::unique_ptr<BaseEntity> Content::mapFields(StringMap *e)
{
}

std::vector<std::string> Content::tableViewColumns() const
{
    return tableViewCols<std::string>(
                name()->displayName(),
                contentDisplayName()->displayName()
                );
}

std::vector<std::string> Content::tableViewValues()
{
    return tableViewCols<std::string>(
                name()->valueToString(),
                contentDisplayName()->valueToString()
                );
}

QStringList Content::tableHeaders() const
{
    return mHeader;
}

std::string Content::tableName() const
{
    return mTableName;
}

void Content::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::string Content::searchColumn() const
{
    return name()->valueToString();
}

void Content::populateEntity()
{
}

std::unique_ptr<BaseEntity> Content::cloneAsUnique()
{
    return std::make_unique<Content>();
}

void Content::afterMapping(BaseEntity& /*entity*/)
{
}
