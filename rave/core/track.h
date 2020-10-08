#ifndef TRACK_H
#define TRACK_H

#include "../framework/baseentity.h"


class Track : public BaseEntity
{
public:
    Track();
    ~Track() override;

    StringField* title() const;
    void setTitle(const std::string track_title);

    StringField* filepath() const;
    void setFilepath(const std::string fpath);

    ForeignKeyField* artist() const;
    void setArtist(int val);

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
    StringField* mTitle;
    StringField* mFilepath;
    ForeignKeyField* mArtist;

    QStringList mHeader;
    std::string mTableName;
};

#endif // TRACK_H
