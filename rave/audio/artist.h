#ifndef ARTIST_H
#define ARTIST_H

#include "../framework/baseentity.h"

template<typename T>
class ChoiceField;

namespace AUDIO{

    class Artist : public BaseEntity
    {
    public:
        Artist();
        ~Artist() override;

        Artist& operator=(const Artist& other);


        StringField* first_name() const;
        void set_first_name(const std::string fname);

        StringField* surname() const;
        void set_surname(const std::string sname);

        StringField* fullName() const;
        void set_fullname(const std::string fullname);

        ChoiceField<std::string>* artist_type() const;
        void set_artist_type(const std::string artist_type);

        TextField* notes() const;
        void set_notes(const std::string nts);

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

    private:
        StringField* m_first_name;
        StringField* m_surname;
        StringField* m_fullname;
        ChoiceField<std::string>* m_artist_type;
        TextField* m_notes;

        QStringList mHeader;
        std::string mTableName;
    };

}// namespace Audio

#endif // ARTIST_H
