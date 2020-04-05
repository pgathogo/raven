#include "entityfields.h"

Field::Field()
    :mFieldName{"FieldName"},
     mFieldLabel{"FieldLabel"},
     mDBColumnName{"dbColumnName"},
     mVisible{true},
     mFormOnly{false},
     mValue{""}
{
    mValue = "";
}

Field::Field(std::string aName, std::string aLabel)
    :mFieldName{aName},
     mFieldLabel{aLabel},
     mDBColumnName{aName},
     mVisible{true},
     mFormOnly{false},
     mValue{""}
{
    mValue = "";
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

void Field::setValue(const FieldValueType aValue)
{
    mValue = aValue;
}

void Field::setStringValue(const std::string aValue)
{
    mValue = aValue;
}

FieldValueType Field::getValue() const
{
    return mValue;
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

FieldValueType Field::value() const
{
    return mValue;
}

std::string Field::valueToString() const
{
    return "";
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


FieldValueType IntegerField::value() const
{
    return std::get<std::string>(this->getValue());
}

std::string IntegerField::valueToString() const
{
    return std::get<std::string>(this->getValue());
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


FieldValueType StringField::value() const
{
    return std::get<std::string>(this->getValue());
}

std::string StringField::valueToString() const
{
    return std::get<std::string>(this->getValue());
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
FieldValueType TextField::value() const
{
    return std::get<std::string>(this->getValue());
}
std::string TextField::valueToString() const
{
    return std::get<std::string>(this->getValue());
}

