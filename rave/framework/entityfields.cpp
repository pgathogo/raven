#include <QLineEdit>
#include <QTextEdit>

#include "entityfields.h"

#include "../utils/tools.h"

Field::Field()
    :mFieldName{"FieldName"},
     mFieldLabel{"FieldLabel"},
     mDBColumnName{"dbColumnName"},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true},
     mMandatory{false}
{
}

Field::Field(std::string aName, std::string aLabel)
    :mFieldName{aName},
     mFieldLabel{aLabel},
     mDBColumnName{aName},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true},
     mMandatory{false}
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
    ,mWidget{new QLineEdit}
{
}

IntegerField::IntegerField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
    ,mWidget{new QLineEdit}
{
}

IntegerField::~IntegerField()
{
    qDebug() << "IntgerField::dtor";
    delete mWidget;
}


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

QLineEdit* IntegerField::widget()
{
    return mWidget;
}



/* -------- StringField --------- */

StringField::StringField()
        :Field()
        ,mWidget{new QLineEdit}
{
}

StringField::StringField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
    ,mWidget{new QLineEdit}
{
}

StringField::StringField(const StringField& sf)
{
    if (sf.mWidget)
        mWidget = sf.mWidget;
}

StringField& StringField::operator=(const StringField& sf)
{
    if (this != &sf){
        delete mWidget;
        mWidget = sf.mWidget;
    }
    return *this;
}


StringField::~StringField()
{
    qDebug() << "StringField::dtor";
    delete mWidget;
}


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
QLineEdit* StringField::widget()
{
    return mWidget;
}
void StringField::setWidget(QLineEdit* lineEdit)
{
    mWidget = lineEdit;
    mWidget->setText(QString::fromStdString(mValue));
}

void StringField::printWidgetValue()
{
    qDebug() << "Widget Value: " << mWidget->text();
}

void StringField::setValueFromWidget()
{
    mValue = mWidget->text().toStdString();
}

/* ---- TextField ---- */
TextField::TextField()
        :Field()
        ,mWidget{ new QTextEdit }
{
}
TextField::TextField(std::string aName, std::string aLabel)
        :Field(aName, aLabel)
        ,mWidget{ new QTextEdit }
{
}
TextField::~TextField()
{
    qDebug() << "TextField::dtor";
    delete mWidget;
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

QTextEdit* TextField::widget()
{
    return mWidget;
}

void TextField::setWidget(QTextEdit* textEdit)
{
    mWidget = textEdit;
    mWidget->insertPlainText(QString::fromStdString(mValue));
}

void TextField::setValueFromWidget()
{
    mValue = mWidget->toPlainText().toStdString();
}
