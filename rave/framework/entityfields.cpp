#include "entityfields.h"

#include "../utils/tools.h"

Field::Field()
    :mFieldName{"FieldName"},
     mFieldLabel{"FieldLabel"},
     mDBColumnName{"dbColumnName"},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true}
{
}

Field::Field(std::string aName, std::string aLabel)
    :mFieldName{aName},
     mFieldLabel{aLabel},
     mDBColumnName{aName},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true}
{
}

Field::~Field(){}

std::string Field::fieldName() const
{
    return mFieldName;
}

std::string Field::fieldLabel() const
{
    return mFieldLabel;
}

void Field::setName(const std::string aName)
{
    mFieldName = aName;
}

void Field::setLabel(const std::string aLabel)
{
    mFieldLabel = aLabel;
}

std::string Field::dbColumnName() const
{
    return mDBColumnName;
}

void Field::setDBColumnName(std::string aName)
{
    mDBColumnName = aName;
}


bool Field::visible() const
{
    return mVisible;
}

void Field::setVisible(bool value)
{
    mVisible = value;
}

bool Field::formOnly() const
{
    return mFormOnly;
}

void Field::setFormOnly(bool value)
{
    mFormOnly = value;
}
void Field::setSearchable(bool value)
{
    mSearchable = value;
}

bool Field::searchable()
{
    return mSearchable;
}

void Field::setDisplayName(const std::string dispName)
{
    mDisplayName = dispName;
}

std::string Field::displayName() const
{
    return mDisplayName;
}

void Field::setMandatory(bool value)
{
    mMandatory = value;
}

bool Field::mandatory() const
{
    return mMandatory;
}

/* ------ IntegerField ------ */

IntegerField::IntegerField()
    :Field()
{
}

IntegerField::IntegerField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
{
}

IntegerField::~IntegerField(){}


std::string IntegerField::valueToString() const
{
    return std::to_string(mValue);
}

std::string IntegerField::dbValueFormatter()
{
    return valueToString();
}

void IntegerField::setValue(int val)
{
    mValue = val;
}

void IntegerField::stringToValue(std::string val)
{
    mValue = std::stoi(val);
}

QVariant IntegerField::value()
{
    return QVariant(mValue);
}
/* -------- StringField --------- */

StringField::StringField()
        :Field()
{
}

StringField::StringField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
{
}

StringField::~StringField(){}


std::string StringField::valueToString() const
{
    return mValue;
}

std::string StringField::dbValueFormatter()
{
    return "'"+mValue+"'";
}

void StringField::setValue(std::string val)
{
    mValue = val;
}
void StringField::stringToValue(std::string val)
{
    mValue = val;
}
QVariant StringField::value()
{
    return QVariant(stoq(mValue));
}

/* ---- TextField ---- */
TextField::TextField()
        :Field()
{
}
TextField::TextField(std::string aName, std::string aLabel)
        :Field(aName, aLabel)
{
}
TextField::~TextField()
{
}

std::string TextField::valueToString() const
{
    return mValue;
}

std::string TextField::dbValueFormatter()
{
    return "'"+mValue+"'";
}

void TextField::setValue(std::string val)
{
    mValue = val;
}

void TextField::stringToValue(std::string val)
{
    mValue = val;
}

QVariant TextField::value()
{
    return QVariant(QString::fromStdString(mValue));
}
