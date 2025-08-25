#ifndef SPOTMEDIA_H
#define SPOTMEDIA_H

#include "advertmedia.h".h"
#include "../../../rave/framework/manytomany.h"

//namespace AUDIO{
// class Audio;
//}

class CueMarker;

namespace PIXELPLAN{

    class SpotMedia : public ManyToMany
    {
    public:
        SpotMedia();
        SpotMedia(BaseEntity* pEnt, BaseEntity* dEnt);
        ~SpotMedia() override;

        //ForeignKeyField* audio() const;
        DateField* audio_date() const;
        IntegerField* weight() const;
        IntegerField* seqNo() const;

        void set_weight(int val);
        void set_seq_no(int val);

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

        BaseEntity* mtomEntity() override;

        std::shared_ptr<ManyToMany> copy(BaseEntity* parent_entity, BaseEntity* detail_entity) const override;
        void setTable(const std::string tablename) override { setTableName(tablename); }
        IntegerField* parentId() const override;
        void setParentId(int id) override;

        IntegerField* detailId() const override;
        void setDetailId(int id) override;

        BaseEntity* parentEntity() const override;
        BaseEntity* detailEntity() const override;
        void set_detail_entity(BaseEntity* other);

        AdvertMedia& get_pmedia();
        AdvertMedia* advert_media();
        void set_advert_media(AdvertMedia*);

    private:
        AdvertMedia* m_media;

        AdvertMedia p_media;

        DateField* m_media_date;
        IntegerField* m_weight;
        IntegerField* m_seq_no;

        BaseEntity* m_parent_entity;
        BaseEntity* m_detail_entity;

        IntegerField* m_parent_id;
        IntegerField* m_detail_id;

        StringField* m_short_desc;
        TextField* m_notes;

        QStringList m_header;
        std::string m_table_name;

    };

} // PIXELPLAN

#endif // SPOTMEDIA_H
