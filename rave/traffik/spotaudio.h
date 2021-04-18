#ifndef SPOTAUDIO_H
#define SPOTAUDIO_H

#include "../framework/manytomany.h"


class SpotAudio : public ManyToMany
{
public:
    SpotAudio();
    SpotAudio(BaseEntity* pEnt, BaseEntity* dEnt);
    ~SpotAudio() override;

//    ForeignKeyField* spot() const;
//    void setSpot(int val);

    ForeignKeyField* audio() const;
    void set_audio(int val);

    IntegerField* playCount() const;
    void setPlayCount(int val);

    IntegerField* weight() const;
    void setWeight(int val);

    IntegerField* seqNo() const;
    void setSeqNo(int val);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    BaseEntity* mtomEntity() override;

    std::unique_ptr<ManyToMany> copy(BaseEntity* parent_entity, BaseEntity* detail_entity) const override;
    void setTable(const std::string tablename) override { setTableName(tablename); }
    IntegerField* parentId() const override;
    void setParentId(int id) override;

    IntegerField* detailId() const override;
    void setDetailId(int id) override;

    BaseEntity* parentEntity() const override;
    BaseEntity* detailEntity() const override;
    void set_detail_entity(BaseEntity* other);


private:
//    ForeignKeyField* m_spot;
    ForeignKeyField* m_audio;
    IntegerField* m_play_count;
    IntegerField* m_weight;
    IntegerField* m_seq_no;

    BaseEntity* m_parent_entity;
    BaseEntity* m_detail_entity;

    IntegerField* m_parent_id;
    IntegerField* m_detail_id;

    QStringList m_header;
    std::string m_table_name;

};

#endif // SPOTAUDIO_H
