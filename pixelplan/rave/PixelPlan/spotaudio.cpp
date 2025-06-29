#include "spotaudio.h"
#include "spot.h"
// #include "../audio/audio.h"
#include "../audio/audiofile.h"

namespace TRAFFIK {

SpotAudio::SpotAudio()
    : ManyToMany{}, m_audio{nullptr}, p_audio{""}, m_parent_entity{nullptr},
      m_detail_entity{nullptr}, m_parent_id{nullptr}, m_detail_id{nullptr} {
  m_parent_id = createField<IntegerField>("spot_id", "Parent ID");
  m_detail_id = createField<IntegerField>("audio_id", "Detail ID");

  m_parent_entity = this;
  m_detail_entity = new AUDIO::Audio("");

  // m_audio = dynamic_cast<AUDIO::Audio*>(m_detail_entity);

  m_weight = createField<IntegerField>("weight", "Weight");
  m_seq_no = createField<IntegerField>("seq_no", "Seq No");

  m_short_desc = createField<StringField>("short_desc", "Description");
  m_short_desc->setFormOnly(true);

  m_notes = createField<TextField>("notes", "Notes");
  m_notes->setFormOnly(true);

  // m_header << QString::fromStdString(m_audio->title()->fieldLabel())
  //          << QString::fromStdString(m_short_desc->fieldLabel());

  setTableName("rave_spotaudio");
}

SpotAudio::SpotAudio(BaseEntity *pEnt, BaseEntity *dEnt)
    : ManyToMany{}, m_audio{nullptr}, p_audio{""}, m_parent_entity{pEnt},
      m_detail_entity{dEnt} {
  m_parent_id = createField<IntegerField>("spot_id", "Parent ID");
  m_detail_id = createField<IntegerField>("audio_id", "Detail ID");

  //        m_audio = createField<ForeignKeyField>("audio_id", "Audio",
  //                                              std::make_unique<AUDIO::Audio>(""),
  //                                              "title");

  m_audio = dynamic_cast<AUDIO::Audio *>(m_detail_entity);

  m_weight = createField<IntegerField>("weight", "Weight");
  m_seq_no = createField<IntegerField>("seq_no", "Seq No");

  m_short_desc = createField<StringField>("short_desc", "Description");
  m_short_desc->setFormOnly(true);

  m_notes = createField<TextField>("notes", "Notes");
  m_notes->setFormOnly(true);

  m_header << QString::fromStdString(m_audio->title()->fieldLabel())
           << QString::fromStdString(m_short_desc->fieldLabel());

  setTableName("rave_spotaudio");
}

SpotAudio::~SpotAudio() {}

DateField *SpotAudio::audio_date() const { return m_audio_date; }

void SpotAudio::set_audio_date(QDate dt) { m_audio_date->setValue(dt); }

IntegerField *SpotAudio::weight() const { return m_weight; }

void SpotAudio::set_weight(int val) { m_weight->setValue(val); }

IntegerField *SpotAudio::seqNo() const { return m_seq_no; }

void SpotAudio::set_seq_no(int val) { m_seq_no->setValue(val); }

StringField *SpotAudio::short_desc() const { return m_audio->short_desc(); }

TextField *SpotAudio::notes() const { return m_notes; }

StringField *SpotAudio::title() const { return m_audio->title(); }

DecimalField *SpotAudio::duration() const { return m_audio->duration(); }

AUDIO::Audio *SpotAudio::audio() { return m_audio; }

std::string SpotAudio::tableName() const { return m_table_name; }

void SpotAudio::setTableName(const std::string table_name) {
  m_table_name = table_name;
}

std::unique_ptr<BaseEntity> SpotAudio::mapFields(StringMap *raw_entity) {
  return nullptr;
}

std::vector<std::string> SpotAudio::tableViewColumns() const {
  AUDIO::Audio *audio = dynamic_cast<AUDIO::Audio *>(m_detail_entity);

  return tableViewCols<std::string>(audio->title()->displayName(),
                                    audio->short_desc()->displayName());
}

std::vector<std::string> SpotAudio::tableViewValues() {
  AUDIO::Audio *audio = dynamic_cast<AUDIO::Audio *>(m_detail_entity);

  return tableViewCols<std::string>(audio->title()->valueToString(),
                                    audio->short_desc()->valueToString());
}

QStringList SpotAudio::tableHeaders() const { return m_header; }
std::string SpotAudio::searchColumn() const {
  return detailEntity()->searchColumn();
}

void SpotAudio::populateEntity() {}

std::shared_ptr<BaseEntity> SpotAudio::cloneAsShared() {
  return std::make_shared<SpotAudio>(m_parent_entity, m_detail_entity);
}

void SpotAudio::afterMapping(BaseEntity &entity) {
  SpotAudio &spot_audio = dynamic_cast<SpotAudio &>(entity);
  auto audio = spot_audio.detailEntity()->cloneAsShared();
  spot_audio.set_detail_entity(audio.get());

  getEntityById(std::move(audio), spot_audio.detailId()->value());

  // AUDIO::Audio& fetched_audio = const_cast<AUDIO::Audio&>(dynamic_cast<const
  // AUDIO::Audio&>((get_entity())));
  AUDIO::Audio *fetched_audio = const_cast<AUDIO::Audio *>(
      dynamic_cast<const AUDIO::Audio *>(get_entity().get()));
  auto de = spot_audio.detailEntity();
  de = fetched_audio;
}

BaseEntity *SpotAudio::mtomEntity() { return this; }

std::shared_ptr<ManyToMany> SpotAudio::copy(BaseEntity *parent_entity,
                                            BaseEntity *detail_entity) const {
  return std::make_shared<SpotAudio>(parent_entity, detail_entity);
}

IntegerField *SpotAudio::parentId() const { return m_parent_id; }

void SpotAudio::setParentId(int id) { m_parent_id->setValue(id); }

IntegerField *SpotAudio::detailId() const { return m_detail_id; }

void SpotAudio::setDetailId(int id) { m_detail_id->setValue(id); }

BaseEntity *SpotAudio::parentEntity() const { return m_parent_entity; }

BaseEntity *SpotAudio::detailEntity() const { return m_detail_entity; }

void SpotAudio::set_detail_entity(BaseEntity *other) {
  m_detail_entity = other;
}

AUDIO::Audio &SpotAudio::get_paudio() { return p_audio; }

void SpotAudio::set_title(std::string title) {
  m_audio->set_title(title);

  /*
  auto audio = static_cast<AUDIO::Audio*>(m_audio->fk_entity());

  if (audio != nullptr){
      qDebug() << "Setting title";
      audio->set_title(title);
  }else{
      qDebug() << "set_title failed";
  }
  */
}

void SpotAudio::set_short_desc(std::string sdesc) {
  m_audio->set_short_desc(sdesc);
  //        m_short_desc->setValue(sdesc);
}

void SpotAudio::set_notes(const std::string notes) { m_notes->setValue(notes); }

void SpotAudio::set_marker(CueMarker marker) {
  m_audio->set_cue_marker(marker);
}

void SpotAudio::set_audio_type(std::string audio_type) {
  m_audio->set_audio_type(audio_type);
}

void SpotAudio::set_audio_lib_path(const std::string lib_path) {
  m_audio->set_audio_lib_path(lib_path);
}

void SpotAudio::set_duration(int duration) { m_audio->set_duration(duration); }

void SpotAudio::set_file_path(const std::string file_path) {
  m_audio->set_file_path(file_path);
}

void SpotAudio::set_folder(int folder) { m_audio->set_folder(folder); }

void SpotAudio::set_genre(int genre) { m_audio->set_genre(genre); }

void SpotAudio::set_mood(int mood) { m_audio->set_mood(mood); }

void SpotAudio::set_artist(int id) { m_audio->set_artist(id); }

void SpotAudio::set_audio_year(int yr) { m_audio->set_audio_year(yr); }

} // namespace TRAFFIK
