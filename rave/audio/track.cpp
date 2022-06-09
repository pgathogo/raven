#include "track.h"
#include "../audio/artist.h"

namespace AUDIO{

    Track::Track()
    {
        mTitle = createField<StringField>("title", "Title");
        mTitle->setMandatory(true);

        mFilepath = createField<StringField>("filepath", "File Path");

        mArtist = createField<ForeignKeyField>("artist_id", "Artist",
                                               std::make_unique<Artist>(), "fullname");

        mHeader << QString::fromStdString(mTitle->fieldLabel())
                << QString::fromStdString(mFilepath->fieldLabel())
                << QString::fromStdString(mArtist->fieldLabel());

        setTableName("rave_track");
    }

    Track::~Track()
    {
    }

    StringField *Track::title() const
    {
        return mTitle;
    }

    void Track::setTitle(const std::string track_title)
    {
        mTitle->setValue(track_title);
    }

    StringField *Track::filepath() const
    {
        return mFilepath;
    }

    void Track::setFilepath(const std::string fpath)
    {
        mFilepath->setValue(fpath);
    }

    ForeignKeyField *Track::artist() const
    {
        return mArtist;
    }

    void Track::setArtist(int val)
    {
        mArtist->setValue(val);
    }

    std::string Track::tableName() const
    {
        return mTableName;
    }

    void Track::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::unique_ptr<BaseEntity> Track::mapFields(StringMap *raw_entity)
    {
    }

    std::vector<std::string> Track::tableViewColumns() const
    {
        return tableViewCols<std::string>(
                   title()->displayName(),
                   filepath()->displayName(),
                   artist()->displayName()
                    );
    }

    std::vector<std::string> Track::tableViewValues()
    {
        return tableViewColumns();
    }

    QStringList Track::tableHeaders() const
    {
        return mHeader;
    }

    std::string Track::searchColumn() const
    {
        return title()->valueToString();
    }

    void Track::populateEntity()
    {
    }

    std::unique_ptr<BaseEntity> Track::cloneAsUnique()
    {
        return std::make_unique<Track>();
    }

    void Track::afterMapping(BaseEntity &entity)
    {
    }

}// namespace Audio
