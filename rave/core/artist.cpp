#include "artist.h"

#include "../framework/choicefield.h"

Artist::Artist()
{
    mFirstName = createField<StringField>("first_name", "First Name");
    mFirstName->setMandatory(true);

    mSurname = createField<StringField>("surname", "Surname");

    mFullName = createField<StringField>("fullname", "Full Name");
    mFullName->setValue(mFirstName->value() + mSurname->value());

    mArtistType = createField<ChoiceField<std::string>>("artist_type", "Artist Type");
    mArtistType->addChoice({"F", "Female"});
    mArtistType->addChoice({"M", "Male"});
    mArtistType->addChoice({"G", "Group"});

    mNotes = createField<TextField>("notes", "Note");

    mHeader << QString::fromStdString(mFirstName->fieldLabel())
            << QString::fromStdString(mSurname->fieldLabel())
            << QString::fromStdString(mArtistType->fieldLabel());

    setTableName("rave_artist");
}

Artist::~Artist()
{
}

StringField *Artist::firstName() const
{
    return mFirstName;
}

void Artist::setFirstName(const std::string fname)
{
    mFirstName->setValue(fname);
}

StringField *Artist::surname() const
{
    return mSurname;
}

void Artist::setSurname(const std::string sname)
{
    mSurname->setValue( sname );
}

StringField *Artist::fullName() const
{
    return mFullName;
}

void Artist::setFullName(const std::string fullname)
{
    mFullName->setValue(fullname);
}

ChoiceField<std::string> *Artist::artistType() const
{
    return mArtistType;
}

void Artist::setArtistType(const std::string artist_type)
{
    mArtistType->setValue(artist_type);
}

TextField *Artist::notes() const
{
    return mNotes;
}

void Artist::setNotes(const std::string nts)
{
    mNotes->setValue(nts);
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
}

std::vector<std::string> Artist::tableViewColumns() const
{
    return tableViewCols<std::string>(
                firstName()->displayName(),
                surname()->displayName(),
                fullName()->displayName(),
                artistType()->displayName());
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
    return firstName()->valueToString();
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
