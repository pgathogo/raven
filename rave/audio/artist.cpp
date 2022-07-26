#include "artist.h"

#include "../framework/choicefield.h"

namespace AUDIO{

    Artist::Artist()
    {
        m_first_name = createField<StringField>("first_name", "First Name");
        m_first_name->setMandatory(true);

        m_surname = createField<StringField>("surname", "Surname");

        m_fullname = createField<StringField>("fullname", "Full Name");
        m_fullname->setValue(m_first_name->value() + m_surname->value());

        m_artist_type = createField<ChoiceField<std::string>>("artist_type", "Artist Type");
        m_artist_type->addChoice({"F", "Female"});
        m_artist_type->addChoice({"M", "Male"});
        m_artist_type->addChoice({"G", "Group"});

        m_notes = createField<TextField>("notes", "Note");

        mHeader << QString::fromStdString(m_first_name->fieldLabel())
                << QString::fromStdString(m_surname->fieldLabel())
                << QString::fromStdString(m_fullname->fieldLabel())
                << QString::fromStdString(m_artist_type->fieldLabel());

        setTableName("rave_artist");

    }

    Artist& Artist::operator=(const Artist& other)
    {
        if (this == &other) return *this;

        m_first_name->setValue(other.first_name()->value());
        m_first_name->setMandatory(true);

        m_surname->setValue(other.surname()->value());

        m_fullname->setValue(other.fullName()->value());
        m_fullname->setValue(m_first_name->value() + m_surname->value());

        m_artist_type->setValue(other.artist_type()->value());

        m_notes->setValue(other.notes()->value());

        mHeader << QString::fromStdString(m_first_name->fieldLabel())
                << QString::fromStdString(m_surname->fieldLabel())
                << QString::fromStdString(m_fullname->fieldLabel())
                << QString::fromStdString(m_artist_type->fieldLabel());

        setTableName("rave_artist");

        return *this;
    }

    Artist::~Artist()
    {
    }

    StringField *Artist::first_name() const
    {
        return m_first_name;
    }

    void Artist::set_first_name(const std::string fname)
    {
        m_first_name->setValue(fname);
    }

    StringField *Artist::surname() const
    {
        return m_surname;
    }

    void Artist::set_surname(const std::string sname)
    {
        m_surname->setValue( sname );
    }

    StringField *Artist::fullName() const
    {
        return m_fullname;
    }

    void Artist::set_fullname(const std::string fullname)
    {
        m_fullname->setValue(fullname);
    }

    ChoiceField<std::string> *Artist::artist_type() const
    {
        return m_artist_type;
    }

    void Artist::set_artist_type(const std::string artist_type)
    {
        m_artist_type->setValue(artist_type);
    }

    TextField *Artist::notes() const
    {
        return m_notes;
    }

    void Artist::set_notes(const std::string nts)
    {
        m_notes->setValue(nts);
    }

    std::string Artist::tableName() const
    {
        return "rave_artist";
    }

    void Artist::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::unique_ptr<BaseEntity> Artist::mapFields(StringMap *raw_entity)
    {
        return nullptr;
    }

    std::vector<std::string> Artist::tableViewColumns() const
    {
        return tableViewCols<std::string>(
                    first_name()->displayName(),
                    surname()->displayName(),
                    fullName()->displayName(),
                    artist_type()->displayName());
    }

    std::vector<std::string> Artist::tableViewValues()
    {
        return tableViewColumns();
    }

    QStringList Artist::tableHeaders() const
    {
        return mHeader;
    }

    std::string Artist::searchColumn() const
    {
        return fullName()->valueToString();
    }

    void Artist::populateEntity()
    {

    }

    std::unique_ptr<BaseEntity> Artist::cloneAsUnique()
    {
        return std::make_unique<Artist>();
    }

    void Artist::afterMapping(BaseEntity &entity)
    {
    }

}// namespace Audio
