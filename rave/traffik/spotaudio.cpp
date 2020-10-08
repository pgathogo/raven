#include "spotaudio.h"
#include "spot.h"
#include "../core/track.h"

SpotAudio::SpotAudio()
{

    mSpot = createField<ForeignKeyField>("spot_id", "Spot",
                                         std::make_unique<Spot>(), "name");
    mTrack = createField<ForeignKeyField>("track_id", "Track",
                                          std::make_unique<Track>(), "title");
    mPlayCount = createField<IntegerField>("play_count", "Play Count");
    mWeight = createField<IntegerField>("weight", "Weight");
    mSeqNo = createField<IntegerField>("seq_no", "Seq No");

    mHeader << QString::fromStdString(mTrack->fieldLabel())
            << QString::fromStdString(mSpot->fieldLabel());

    setTableName("rave_spotaudio");
}

SpotAudio::~SpotAudio()
{
}

ForeignKeyField *SpotAudio::spot() const
{
    return mSpot;
}

void SpotAudio::setSpot(int val)
{
    mSpot->setValue(val);
}

ForeignKeyField *SpotAudio::track() const
{
    return mTrack;
}

void SpotAudio::setTrack(int val)
{
    mTrack->setValue(val);
}

IntegerField *SpotAudio::playCount() const
{
    return mPlayCount;
}

void SpotAudio::setPlayCount(int val)
{
    mPlayCount->setValue(val);
}

IntegerField *SpotAudio::weight() const
{
    return mWeight;
}

void SpotAudio::setWeight(int val)
{
    mWeight->setValue(val);
}

IntegerField *SpotAudio::seqNo() const
{
    return mSeqNo;
}

void SpotAudio::setSeqNo(int val)
{
    mSeqNo->setValue(val);
}

std::string SpotAudio::tableName() const
{
    return mTableName;
}

void SpotAudio::setTableName(const std::string table_name)
{
    mTableName = table_name;
}

std::unique_ptr<BaseEntity> SpotAudio::mapFields(StringMap *raw_entity)
{
}

std::vector<std::string> SpotAudio::tableViewColumns() const
{
    return tableViewCols<std::string>(
                track()->displayName(),
                spot()->displayName()
                );
}

std::vector<std::string> SpotAudio::tableViewValues()
{
    return tableViewColumns();
}

QStringList SpotAudio::tableHeaders() const
{
    return mHeader;
}

std::string SpotAudio::searchColumn() const
{
    return track()->valueToString();
}

void SpotAudio::populateEntity()
{
}

std::unique_ptr<BaseEntity> SpotAudio::cloneAsUnique()
{
    return std::make_unique<SpotAudio>();
}

void SpotAudio::afterMapping(BaseEntity &entity)
{
}
