#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>

#include "entityfields.h"
#include "entitydatamodel.h"
#include "valuelist.h"

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

//void Field::setDisplayName(const std::string dispName)
//{
//    mDisplayName = dispName;
//}

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
    ,mValue{-1}
    //,mWidget{new QLineEdit}
{
}

IntegerField::IntegerField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
    ,mValue{-1}
    //,mWidget{new QLineEdit}
{
}

IntegerField::~IntegerField()
{
    qDebug() << "IntgerField::dtor";
   // delete mWidget;
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
    setValue(std::stoi(val));
}

int IntegerField::value()
{
    return mValue;
}

/*
QLineEdit* IntegerField::widget()
{
    return mWidget;
}
*/

std::string IntegerField::displayName() const
{
    return valueToString();
}



/* -------- StringField --------- */

StringField::StringField()
{
    qDebug() << "StringField::ctor";
}

StringField::StringField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
{
}

StringField::StringField(const StringField& sf)
{
//    if (sf.mWidget)
//        mWidget->setText(sf.mWidget->text());
    mValue = sf.mValue;
}

StringField& StringField::operator=(const StringField& sf)
{
    qDebug() << "StringField::operator=";
    if (this != &sf){
        //delete mWidget;
        //mWidget = new QLineEdit();
       // mWidget->setText(sf.mWidget->text());
        mValue = sf.mValue;
    }
    return *this;
}


StringField::~StringField()
{
    qDebug() << "StringField::dtor";
   // delete mWidget;
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
    setValue(val);
}
std::string StringField::value()
{
    return mValue;
}
/*
void StringField::setWidget(QLineEdit* lineEdit)
{
    delete mWidget;
    mWidget = lineEdit;
    mWidget->setText(QString::fromStdString(mValue));
}

void StringField::setValueFromWidget()
{
    mValue = mWidget->text().toStdString();
}
*/

std::string StringField::displayName() const
{
    return valueToString();
}

/* ---- TextField ---- */
TextField::TextField()
        :Field()
        //,mWidget{ new QTextEdit }
{
}
TextField::TextField(std::string aName, std::string aLabel)
        :Field(aName, aLabel)
        //,mWidget{ new QTextEdit }
{
}
TextField::~TextField()
{
    qDebug() << "TextField::dtor";
    //delete mWidget;
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

std::string TextField::value()
{
    return mValue;
}

/*
QTextEdit* TextField::widget()
{
    return mWidget;
}

void TextField::setWidget(QTextEdit* textEdit)
{
    delete mWidget;
    mWidget = textEdit;
    mWidget->insertPlainText(QString::fromStdString(mValue));
}

void TextField::setValueFromWidget()
{
    mValue = mWidget->toPlainText().toStdString();
}
*/

std::string TextField::displayName() const
{
    return valueToString();
}

/* -------- LookupField ------------- */

bool LookupField::hasData = false;
EntityDataModel* LookupField::mEDM{};

LookupField::LookupField()
        :Field()
        ,mValue{1}
        ,mIndex{0}
        //,mWidget{ new QComboBox }
{
}

LookupField::LookupField(std::string aName, std::string aLabel, EntityDataModel* edm)
        :Field(aName, aLabel)
        ,mValue{-1}
        ,mIndex{-1}
        //,mWidget{ new QComboBox }
{
    //mWidget->setModel(edm);
    if (!hasData){
        mEDM = edm;
        cacheData();
    }
}

LookupField::~LookupField()
{
    qDebug() << "LookupField::dtor";
    //delete mWidget;
}

std::string LookupField::valueToString() const
{
    return std::to_string(mValue);
}

std::string LookupField::dbValueFormatter()
{
    return std::to_string(mValue);
}

void LookupField::setValue(int val)
{
    mValue = val;
}

void LookupField::stringToValue(std::string val)
{
    mValue = stoi(val);
}

int LookupField::value()
{
    return mValue;
}

EntityDataModel* LookupField::dataModel() const
{
    return mEDM;
}

/*
QComboBox* LookupField::widget()
{
    return mWidget;
}

void LookupField::setWidget(QComboBox* widget)
{
    delete mWidget;
    mWidget = widget;
    mWidget->setModel(mEDM);
    mWidget->setCurrentIndex(mWidget->findText(
                                 QString::fromStdString(this->displayName())));
}

void LookupField::setValueFromWidget()
{
    // get this from the model
    //mValue = mWidget->toPlainText().toStdString();
}
*/

void LookupField::setIndex(int i)
{
    mIndex = i;
}
int LookupField::index() const
{
    return mIndex;
}
void LookupField::setCurrText(std::string txt)
{
    mCurrText = txt;
}
std::string LookupField::currText() const
{
    return mCurrText;
}

void LookupField::cacheData()
{
    if (!hasData){
        mEDM->all();
        if (mEDM->count() > 0)
            hasData = true;
    }
}

std::size_t LookupField::cacheCount()
{
    return mEDM->count();
}

std::string LookupField::displayName() const
{
    std::string name = "NOT-FOUND";

    auto it = mEDM->vecBegin();

    for(; it != mEDM->vecEnd(); ++it){
        if (mValue == std::get<1>(*it)->id()){
            ValueList* vl = dynamic_cast<ValueList*>(std::get<1>(*it).get());
            name = vl->listValue()->displayName();
        }
    }

    return name;
}

/*
ChoiceField::ChoiceField()
    :mIndex{-1}
    //:mWidget{}
{
}

ChoiceField::ChoiceField(std::string aName, std::string aLabel)
    :Field(aName, aLabel)
    ,mIndex{-1}
    //,mWidget{new QComboBox}
{
}

ChoiceField::~ChoiceField()
{
    //delete mWidget;
}

std::string ChoiceField::valueToString() const
{
    return mValue;
}

std::string ChoiceField::dbValueFormatter()
{
    return mValue;
}

void ChoiceField::stringToValue(std::string val)
{
}

std::string ChoiceField::value()
{
    return mValue;
}

std::string ChoiceField::displayName() const
{
    std::string name = "NOT-FOUND";

    for (auto& c : mChoices){
        if (mValue == std::get<0>(c)){
            name = std::get<1>(c);
        }
    }

    return name;
}

void ChoiceField::addChoice(Choice choice)
{
    mChoices.push_back(choice);
    //mWidget->addItem(QString::fromStdString(std::get<1>(choice)));
}

void ChoiceField::setValue(std::string value)
{
    mValue = value;
}

void ChoiceField::setIndex(int i)
{
    mIndex = i;
}

int ChoiceField::index() const
{
    return mIndex;
}

void ChoiceField::setCurrText(std::string text)
{
    mCurrText = text;
}

std::string ChoiceField::currText() const
{
    return mCurrText;
}
*/

/* ----- ForeignKeyField ------- */
ForeignKeyField::ForeignKeyField(const std::string aName, const std::string aLabel,
                std::unique_ptr<BaseEntity> fkEntity)
    :Field{aName, aLabel}
    ,mFKEntity{std::move(fkEntity)}
    ,mValue{-1}
{
}

ForeignKeyField::~ForeignKeyField()
{
}
std::string ForeignKeyField::valueToString() const
{
    return std::to_string(mValue);
}

std::string ForeignKeyField::dbValueFormatter()
{
    return std::to_string(mValue);
}

void ForeignKeyField::stringToValue(std::string val)
{
    mValue = stoi(val);
}

void ForeignKeyField::setValue(int val)
{
    mValue = val;
}

int ForeignKeyField::value()
{
    return mValue;
}

std::string ForeignKeyField::displayName() const
{
    return "";
}
