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

Field::Field(const std::string aName, const std::string aLabel)
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

IntegerField::IntegerField(const std::string aName, const std::string aLabel)
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
    if (!val.empty())
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

/* ------- DecimalField ------------ */
DecimalField::DecimalField()
{
}

DecimalField::DecimalField(const std::string aName, const std::string aLabel)
    :Field{aName, aLabel}
    ,mValue{0.0}
{
}
DecimalField::~DecimalField()
{
}
std::string DecimalField::valueToString() const
{
    return std::to_string(mValue);
}
std::string DecimalField::dbValueFormatter()
{
    return std::to_string(mValue);
}
void DecimalField::stringToValue(const std::string /*val*/)
{
}
void DecimalField::setValue(double val)
{
    mValue = val;
}
double DecimalField::value()
{
    return mValue;
}
std::string DecimalField::displayName() const
{
    return valueToString();
}


/* -------- StringField --------- */

StringField::StringField()
{
    qDebug() << "StringField::ctor";
}

StringField::StringField(const std::string aName, const std::string aLabel)
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
TextField::TextField(const std::string aName, const std::string aLabel)
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

std::map<std::string, std::unique_ptr<EntityDataModel>> LookupField::lookups;


LookupField::LookupField()
        :Field()
        ,mValue{1}
        ,mIndex{0}
{
}

LookupField::LookupField(const std::string aName,
                         const std::string aLabel,
                         BaseEntity* lookupEntity)
        :Field{aName, aLabel}
        ,mValue{-1}
        ,mIndex{-1}
{

    auto it = lookups.find(aName);
    if (it == lookups.end()){
        lookups[aName]= std::make_unique<EntityDataModel>(lookupEntity);
        lookups[aName]->all();
    }


}

LookupField::~LookupField()
{
    qDebug() << "LookupField::dtor";
}

std::string LookupField::valueToString() const
{
    return std::to_string(mValue);
}

std::string LookupField::dbValueFormatter()
{
    std::string result;

    if (mValue == -1){
        result = "null";
    }else{
        result = std::to_string(mValue);
    }

    return result;

}

void LookupField::setValue(int val)
{
    mValue = val;
}

void LookupField::stringToValue(std::string val)
{
    if (!val.empty())
        mValue = stoi(val);
}

int LookupField::value() const
{
    return mValue;
}

EntityDataModel* LookupField::dataModel() const
{
    return lookups[fieldName()].get();
}

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
    /*
    if (!hasData){
        mEDM->all();
        if (mEDM->count() > 0)
            hasData = true;
    }
    */
}

std::size_t LookupField::cacheCount()
{
    return lookups[fieldName()]->count();
}

std::string LookupField::displayName() const
{
    std::string name = "NOT-FOUND";

    auto it = lookups[fieldName()]->vecBegin();

    for(; it != lookups[fieldName()]->vecEnd(); ++it){
        if (mValue == std::get<1>(*it)->id()){
            ValueList* vl = dynamic_cast<ValueList*>(std::get<1>(*it).get());
            name = vl->listValue()->displayName();
        }
    }

    return name;
}


/* ----- ForeignKeyField ------- */


ForeignKeyField::ForeignKeyField(const std::string aName, const std::string aLabel,
                BaseEntity* fkEntity, const std::string dispName)
    :LookupField{aName, aLabel, fkEntity}
     ,mDisplayName{dispName}
{
}

ForeignKeyField::~ForeignKeyField()
{
}

std::string ForeignKeyField::displayName() const
{
    std::string name = "NOT-FOUND";
    bool found = false;

    auto it = dataModel()->vecBegin();

    for(; it != dataModel()->vecEnd(); ++it){

        if (found)
            break;

        if (value() == std::get<1>(*it)->id()){
            BaseEntity* entity = dynamic_cast<BaseEntity*>(std::get<1>(*it).get());

            for (const auto& tup: entity->fields()){
                if (std::get<0>(tup) == mDisplayName){
                    name = std::get<1>(tup)->displayName();
                    found = true;
                    break;
                }
            }
        }
    }

    return name;
}


