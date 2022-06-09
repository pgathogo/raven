#include "audiofolder.h"

AudioFolder::AudioFolder()
    :m_folder_name{}
    ,m_parent_id{}
{
    m_folder_name = createField<StringField>("folder_name", "Folder Name");
    m_folder_name->setMandatory(true);

    m_parent_id = createField<IntegerField>("parent", "Parent Id");

    m_header << QString::fromStdString(m_folder_name->fieldLabel());

    setTableName("rave_folder");
}

AudioFolder::~AudioFolder()
{
}

StringField *AudioFolder::folder_name() const
{
    return m_folder_name;
}

IntegerField *AudioFolder::parent_id() const
{
    return m_parent_id;
}

void AudioFolder::set_folder_name(std::string folder_name)
{
    m_folder_name->setValue(folder_name);
}

void AudioFolder::set_parent_id(int parent_id)
{
    m_parent_id->setValue(parent_id);
}

std::string AudioFolder::tableName() const
{
    return m_table_name;
}

void AudioFolder::setTableName(const std::string table_name)
{
    m_table_name = table_name;
}

std::unique_ptr<BaseEntity> AudioFolder::mapFields(StringMap* raw_entity)
{
}

std::vector<std::string> AudioFolder::tableViewColumns() const
{
    return tableViewCols<std::string>(
            folder_name()->displayName()
                );
}

std::vector<std::string> AudioFolder::tableViewValues()
{
    return tableViewColumns();
}

QStringList AudioFolder::tableHeaders() const
{
    return m_header;
}

std::string AudioFolder::searchColumn() const
{
    return m_folder_name->value();
}

void AudioFolder::populateEntity()
{
}

std::unique_ptr<BaseEntity> AudioFolder::cloneAsUnique()
{
    return std::make_unique<AudioFolder>();
}

void AudioFolder::afterMapping(BaseEntity& entity)
{
}
