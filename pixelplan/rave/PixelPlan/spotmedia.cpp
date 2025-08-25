#include "spotmedia.h"
#include "spot.h"
// #include "../audio/audio.h"
#include "../audio/audiofile.h"

namespace PIXELPLAN {

SpotMedia::SpotMedia()
    : ManyToMany{}
    , m_media{nullptr}
    , p_media{}
    , m_parent_entity{nullptr}
    , m_detail_entity{nullptr}
    , m_parent_id{nullptr}
    , m_detail_id{nullptr}
{
  m_parent_id = createField<IntegerField>("spot_id", "Parent ID");
  m_detail_id = createField<IntegerField>("media_id", "Detail ID");

  m_parent_entity = this; // I thought the parent is the Spot
  m_detail_entity = new AdvertMedia();

  m_weight = createField<IntegerField>("weight", "Weight");
  m_seq_no = createField<IntegerField>("seq_no", "Seq No");

  setTableName("rave_spotmedia");
}

SpotMedia::SpotMedia(BaseEntity *pEnt, BaseEntity *dEnt)
    : ManyToMany{}
    , m_media{nullptr}
    , p_media{}
    , m_parent_entity{pEnt}
    , m_detail_entity{dEnt}
{
  m_parent_id = createField<IntegerField>("spot_id", "Parent ID");
  m_detail_id = createField<IntegerField>("media_id", "Detail ID");

  m_media = dynamic_cast<AdvertMedia*>(m_detail_entity);

  m_weight = createField<IntegerField>("weight", "Weight");
  m_seq_no = createField<IntegerField>("seq_no", "Seq No");

  m_header << QString::fromStdString(m_media->title()->fieldLabel());

  setTableName("rave_spotmedia");
}

SpotMedia::~SpotMedia() {}

AdvertMedia* SpotMedia::advert_media() { return m_media; }

void SpotMedia::set_advert_media(AdvertMedia* am)
{
    m_media = am;
}

std::string SpotMedia::tableName() const { return m_table_name; }

void SpotMedia::setTableName(const std::string table_name)
{
  m_table_name = table_name;
}

std::unique_ptr<BaseEntity> SpotMedia::mapFields(StringMap *raw_entity)
{
  return nullptr;
}

std::vector<std::string> SpotMedia::tableViewColumns() const
{
   AdvertMedia* media = dynamic_cast<AdvertMedia*>(m_detail_entity);

    return tableViewCols<std::string>(media->title()->displayName());
}

std::vector<std::string> SpotMedia::tableViewValues()
{
  AdvertMedia* media = dynamic_cast<AdvertMedia*>(m_detail_entity);

  return tableViewCols<std::string>(media->title()->valueToString());
}

QStringList SpotMedia::tableHeaders() const { return m_header; }

std::string SpotMedia::searchColumn() const
{
  return detailEntity()->searchColumn();
}

void SpotMedia::populateEntity() {}

std::shared_ptr<BaseEntity> SpotMedia::cloneAsShared() {
  return std::make_shared<SpotMedia>(m_parent_entity, m_detail_entity);
}

void SpotMedia::afterMapping(BaseEntity &entity) {
  SpotMedia &spot_media = dynamic_cast<SpotMedia &>(entity);
  auto media = spot_media.detailEntity()->cloneAsShared();
  spot_media.set_detail_entity(media.get());

  getEntityById(std::move(media), spot_media.detailId()->value());

  AdvertMedia* fetched_media = const_cast<AdvertMedia*>(
      dynamic_cast<const AdvertMedia*>(get_entity().get()));
  auto de = spot_media.detailEntity();
  de = fetched_media;
}

BaseEntity* SpotMedia::mtomEntity() { return this; }

std::shared_ptr<ManyToMany> SpotMedia::copy(BaseEntity *parent_entity,
                                            BaseEntity *detail_entity) const {
  return std::make_shared<SpotMedia>(parent_entity, detail_entity);
}

IntegerField* SpotMedia::parentId() const { return m_parent_id; }

void SpotMedia::setParentId(int id) { m_parent_id->setValue(id); }

IntegerField* SpotMedia::detailId() const { return m_detail_id; }

void SpotMedia::setDetailId(int id) { m_detail_id->setValue(id); }

BaseEntity* SpotMedia::parentEntity() const { return m_parent_entity; }

BaseEntity* SpotMedia::detailEntity() const { return m_detail_entity; }

void SpotMedia::set_detail_entity(BaseEntity *other)
{
  m_detail_entity = other;
}

//AdvertMedia& SpotMedia::get_pmedia() { return p_media; }
AdvertMedia& SpotMedia::get_pmedia() { return *m_media; }

} // namespace TRAFFIK
