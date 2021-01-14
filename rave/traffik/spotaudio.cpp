#include "spotaudio.h"
#include "spot.h"
#include "../core/track.h"

SpotAudio::SpotAudio()
{

    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
                                         std::make_unique<TRAFFIK::Spot>(), "name");
    m_track = createField<ForeignKeyField>("track_id", "Track",
                                          std::make_unique<Track>(), "title");
    m_play_count = createField<IntegerField>("play_count", "Play Count");
    m_weight = createField<IntegerField>("weight", "Weight");
    m_seq_no = createField<IntegerField>("seq_no", "Seq No");

    mHeader << QString::fromStdString(m_track->fieldLabel())
            << QString::fromStdString(m_spot->fieldLabel());

    setTableName("rave_spotaudio");
}

SpotAudio::~SpotAudio()
{
}

ForeignKeyField *SpotAudio::spot() const
{
    return m_spot;
}

void SpotAudio::setSpot(int val)
{
    m_spot->setValue(val);
}

ForeignKeyField *SpotAudio::track() const
{
    return m_track;
}

void SpotAudio::setTrack(int val)
{
    m_track->setValue(val);
}

IntegerField *SpotAudio::playCount() const
{
    return m_play_count;
}

void SpotAudio::setPlayCount(int val)
{
    m_play_count->setValue(val);
}

IntegerField *SpotAudio::weight() const
{
    return m_weight;
}

void SpotAudio::setWeight(int val)
{
    m_weight->setValue(val);
}

IntegerField *SpotAudio::seqNo() const
{
    return m_seq_no;
}

void SpotAudio::setSeqNo(int val)
{
    m_seq_no->setValue(val);
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
