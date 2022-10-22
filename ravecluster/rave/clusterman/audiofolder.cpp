#include "audiofolder.h"

#include "storagedisk.h"
#include "cluster.h"

namespace ClusterManager
{
    AudioFolder::AudioFolder()
    {
        qDebug() << " 111 ";

        m_folder_name = createField<StringField>("folder_name", "Folder Name");

        qDebug() << " 222 ";

        m_disk = createField<ForeignKeyField>("disk_id", "Storage Disk",
                                             std::make_unique<Cluster>(), "disk_name");

        qDebug() << " 333 ";

        m_header << QString::fromStdString(m_folder_name->fieldName())
                 << QString::fromStdString(m_disk->fieldName());

        qDebug() << " 444 ";

        setTableName("rave_audiofolder");
    }

    AudioFolder::~AudioFolder()
    {
    }

    StringField* AudioFolder::folder_name() const
    {
        return m_folder_name;
    }

    ForeignKeyField* AudioFolder::disk()
    {
        return m_disk;
    }

    void AudioFolder::set_folder_name(const std::string fld_name)
    {
        m_folder_name->setValue(fld_name);
    }

    void AudioFolder::set_disk(int disk_id)
    {
        m_disk->setValue(disk_id);
    }

    std::string AudioFolder::tableName() const
    {
        return "rave_audiofolder";
    }

    std::unique_ptr<BaseEntity> AudioFolder::mapFields(StringMap*)
    {
    }

    std::vector<std::string> AudioFolder::tableViewColumns() const
    {
        return tableViewCols<std::string>(folder_name()->displayName());
    }

    std::vector<std::string> AudioFolder::tableViewValues()
    {
        return {folder_name()->displayName()};
    }

    QStringList AudioFolder::tableHeaders() const
    {
        return m_header;
    }

    std::string AudioFolder::searchColumn() const
    {
        return folder_name()->valueToString();
    }

    std::unique_ptr<BaseEntity> AudioFolder::cloneAsUnique()
    {
        return std::make_unique<AudioFolder>();
    }

    void AudioFolder::populateEntity()
    {
    }

    void AudioFolder::afterMapping(BaseEntity&)
    {
    }

    void AudioFolder::setTableName(const std::string table)
    {
        m_table_name = table;
    }


}
