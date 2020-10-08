#ifndef ARTIST_H
#define ARTIST_H

#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;


class Artist : public BaseEntity
{
public:
    Artist();
    ~Artist() override;

    StringField* firstName() const;
    void setFirstName(const std::string fname);

    StringField* surname() const;
    void setSurname(const std::string sname);

    StringField* fullName() const;
    void setFullName(const std::string fullname);

    ChoiceField<std::string>* artistType() const;
    void setArtistType(const std::string artist_type);

    TextField* notes() const;
    void setNotes(const std::string nts);

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
    StringField* mFirstName;
    StringField* mSurname;
    StringField* mFullName;
    ChoiceField<std::string>* mArtistType;
    TextField* mNotes;

    QStringList mHeader;
    std::string mTableName;
};

#endif // ARTIST_H
