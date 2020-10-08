#ifndef SPOTAUDIO_H
#define SPOTAUDIO_H

#include "../framework/baseentity.h"


class SpotAudio : public BaseEntity
{
public:
    SpotAudio();
    ~SpotAudio() override;

    ForeignKeyField* spot() const;
    void setSpot(int val);

    ForeignKeyField* track() const;
    void setTrack(int val);

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

private:
    ForeignKeyField* mSpot;
    ForeignKeyField* mTrack;
    IntegerField* mPlayCount;
    IntegerField* mWeight;
    IntegerField* mSeqNo;

    QStringList mHeader;
    std::string mTableName;

};

#endif // SPOTAUDIO_H
