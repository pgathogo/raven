#include "spotaudio.h"
#include "spot.h"
#include "../audio/audio.h"

SpotAudio::SpotAudio()
    :ManyToMany{}
{

//    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
//                                         std::make_unique<TRAFFIK::Spot>(), "name");
    m_audio = createField<ForeignKeyField>("audio_id", "Audio",
                                          std::make_unique<AUDIO::Audio>(""), "title");
    m_play_count = createField<IntegerField>("play_count", "Play Count");
    m_weight = createField<IntegerField>("weight", "Weight");
    m_seq_no = createField<IntegerField>("seq_no", "Seq No");

    m_header << QString::fromStdString(m_audio->fieldLabel())
            << QString::fromStdString(m_weight->fieldLabel());

    setTableName("rave_spotaudio");
}

SpotAudio::SpotAudio(BaseEntity *pEnt, BaseEntity *dEnt)
    :ManyToMany{},
     m_parent_entity{pEnt},
     m_detail_entity{dEnt}
{
    m_parent_id = createField<IntegerField>("spot_id","Parent ID");
    m_detail_id = createField<IntegerField>("audio_id","Detail ID");

//    m_spot = createField<ForeignKeyField>("spot_id", "Spot",
//                                         std::make_unique<TRAFFIK::Spot>(), "name");

    m_audio = createField<ForeignKeyField>("audio_id", "Audio",
                                          std::make_unique<AUDIO::Audio>(""), "title");
    m_play_count = createField<IntegerField>("play_count", "Play Count");
    m_weight = createField<IntegerField>("weight", "Weight");
    m_seq_no = createField<IntegerField>("seq_no", "Seq No");

    m_header << QString::fromStdString(m_audio->fieldLabel())
             << QString::fromStdString(m_weight->fieldLabel());

    setTableName("rave_spotaudio");

}

SpotAudio::~SpotAudio()
{
}

/*
ForeignKeyField *SpotAudio::spot() const
{
    return m_spot;
}
*/

/*
void SpotAudio::setSpot(int val)
{
    m_spot->setValue(val);
}
*/

ForeignKeyField *SpotAudio::audio() const
{
    return m_audio;
}

void SpotAudio::set_audio(int val)
{
    m_audio->setValue(val);
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
    return m_table_name;
}

void SpotAudio::setTableName(const std::string table_name)
{
    m_table_name = table_name;
}

std::unique_ptr<BaseEntity> SpotAudio::mapFields(StringMap *raw_entity)
{
}

std::vector<std::string> SpotAudio::tableViewColumns() const
{
    return tableViewCols<std::string>( audio()->displayName() );
}

std::vector<std::string> SpotAudio::tableViewValues()
{
    return tableViewColumns();
}

QStringList SpotAudio::tableHeaders() const
{
    return m_header;
}

std::string SpotAudio::searchColumn() const
{
    return audio()->valueToString();
}

void SpotAudio::populateEntity()
{
}

std::unique_ptr<BaseEntity> SpotAudio::cloneAsUnique()
{
    return std::make_unique<SpotAudio>(m_parent_entity, m_detail_entity);
}

void SpotAudio::afterMapping(BaseEntity &entity)
{
    SpotAudio& spot_audio = dynamic_cast<SpotAudio&>(entity);
    auto sa = spot_audio.detailEntity()->cloneAsUnique();
    spot_audio.set_detail_entity(sa.get());
    getEntityById(std::move(sa), spot_audio.detailId()->value());
}

BaseEntity *SpotAudio::mtomEntity()
{
    return this;
}

std::unique_ptr<ManyToMany> SpotAudio::copy(BaseEntity *parent_entity, BaseEntity *detail_entity) const
{
    return std::make_unique<SpotAudio>(parent_entity, detail_entity);
}

IntegerField *SpotAudio::parentId() const
{
    return m_parent_id;
}

void SpotAudio::setParentId(int id)
{
    m_parent_id->setValue( id );
}

IntegerField *SpotAudio::detailId() const
{
    return m_detail_id;
}

void SpotAudio::setDetailId(int id)
{
    m_detail_id->setValue(id);
}

BaseEntity *SpotAudio::parentEntity() const
{
    return m_parent_entity;
}

BaseEntity *SpotAudio::detailEntity() const
{
    return m_detail_entity;
}

void SpotAudio::set_detail_entity(BaseEntity *other)
{
    m_detail_entity = other;

}
