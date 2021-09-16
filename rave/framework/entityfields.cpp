#include <chrono>

#include <QDebug>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QTime>

#include "entityfields.h"
#include "entitydatamodel.h"
#include "valuelist.h"

#include "../utils/tools.h"
#include "../lib/date/date.h"

Field::Field()
    :mFieldName{"FieldName"},
     mFieldLabel{"FieldLabel"},
     mDBColumnName{"dbColumnName"},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true},
     mMandatory{false},
     mReadOnly{false}
{
}

Field::Field(const std::string aName, const std::string aLabel)
    :mFieldName{aName},
     mFieldLabel{aLabel},
     mDBColumnName{aName},
     mVisible{true},
     mFormOnly{false},
     mSearchable{true},
     mMandatory{false},
     mReadOnly{false}
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

bool Field::readOnly() const
{
    return mReadOnly;
}

void Field::setReadOnly(bool value)
{
    mReadOnly = value;
}

void Field::setSearchable(bool value)
{
    mSearchable = value;
}

bool Field::searchable()
{
    return mSearchable;
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
    ,mValue{-1}
{
}

IntegerField::IntegerField(const std::string aName, const std::string aLabel)
    :Field(aName, aLabel)
    ,mValue{-1}
{
}

IntegerField::~IntegerField()
{
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


std::string IntegerField::displayName() const
{
    return valueToString();
}

/* ------- DecimalField ------------ */
DecimalField::DecimalField()
    :mValue{0.0}
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
void DecimalField::stringToValue(const std::string val)
{
    if (val.empty())
        mValue = 0.0;
    else
        mValue = std::stod(val);
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

/* -------- BooleanField --------*/

BooleanField::BooleanField()
    :mValue{false}
{}

BooleanField::BooleanField(const std::string aName, const std::string aLabel)
    :Field{aName, aLabel}
    ,mValue{false}
{
}

BooleanField::~BooleanField()
{
}

std::string BooleanField::valueToString() const
{
    return mValue ? "true" : "false";
}

std::string BooleanField::dbValueFormatter()
{
    return mValue ? "true" : "false";
}

void BooleanField::stringToValue(const std::string val)
{
    if (val.empty()) return;

    std::locale loc;

    for(auto s: val)
        std::tolower(s);

    if (val == "t" || val == "true")
        mValue = true;
    else
        mValue = false;
}

void BooleanField::setValue(bool val)
{
    mValue = val;
}

bool BooleanField::value() const
{
    return mValue;
}

std::string BooleanField::displayName() const
{
    return mValue ? "True" : "False";
}


/* -------- StringField --------- */

StringField::StringField()
    :mValue{}
{
}

StringField::StringField(const std::string aName, const std::string aLabel)
    :Field(aName, aLabel),
      mValue{}
{
}

StringField::StringField(const StringField& sf)
{
    mValue = sf.mValue;
}

StringField& StringField::operator=(const StringField& sf)
{
    if (this != &sf){
        mValue = sf.mValue;
    }
    return *this;
}


StringField::~StringField()
{
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


std::string StringField::displayName() const
{
    return valueToString();
}

/* ---- TextField ---- */
TextField::TextField()
        :Field()
{
}
TextField::TextField(const std::string aName, const std::string aLabel)
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

std::string TextField::value()
{
    return mValue;
}

std::string TextField::displayName() const
{
    return valueToString();
}

/* -------- LookupField ------------- */

std::map<std::string, std::unique_ptr<EntityDataModel>> ForeignKeyField::lookups;


ForeignKeyField::ForeignKeyField()
        :Field()
        ,mValue{1}
        ,mIndex{0}
        ,mDisplayField{}
{
}

ForeignKeyField::ForeignKeyField(const std::string aName, const std::string aLabel,
                std::unique_ptr<BaseEntity> fkEntity,
                                 const std::string displayField,
                                 const std::string filter)
        :Field{aName, aLabel}
        ,mValue{-1}
        ,mIndex{-1}
        ,mDisplayField{displayField}
        ,m_fk_entity{fkEntity.get()}
{
    auto it = lookups.find(aName);

    if (it == lookups.end()){
        lookups[aName]= std::make_unique<EntityDataModel>(std::move(fkEntity));
        if (filter.empty()){
            lookups[aName]->all();
        }else{
            lookups[aName]->search(filter);
        }
    }else{
        if (!filter.empty()){
            lookups[aName]->clear();
            lookups[aName]->search(filter);
        }
    }

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
    std::string result;

    if (mValue == -1){
        result = "null";
    }else{
        result = std::to_string(mValue);
    }

    return result;

}

void ForeignKeyField::setValue(int val)
{
    mValue = val;
    //setCurrentText();
}

void ForeignKeyField::stringToValue(std::string val)
{
    if (!val.empty())
        mValue = stoi(val);
}

int ForeignKeyField::value() const
{
    return mValue;
}

EntityDataModel* ForeignKeyField::dataModel() const
{
    return lookups[fieldName()].get();
}

std::string ForeignKeyField::sourceTableName() const
{
    return dataModel()->entityTableName();
}

BaseEntity *ForeignKeyField::currentEntity()
{
    BaseEntity* ent = nullptr;

    for (auto&[name, entity] : dataModel()->modelEntities()){
        if (name == currText()){
            ent = entity.get();
            break;
        }
    }

    return ent;
}

BaseEntity* ForeignKeyField::fk_entity()
{
    return m_fk_entity;
}

void ForeignKeyField::set_fk_entity(std::unique_ptr<BaseEntity> ent)
{
    m_unique_fk_entity = std::move(ent);
}

BaseEntity* ForeignKeyField::unique_fk_entity()
{
    return m_unique_fk_entity.get();
}


void ForeignKeyField::setIndex(int i)
{
    mIndex = i;
}
int ForeignKeyField::index() const
{
    return mIndex;
}
void ForeignKeyField::setCurrText(std::string txt)
{
    mCurrText = txt;
}

std::string ForeignKeyField::currText() const
{
    return mCurrText;
}

void ForeignKeyField::setCurrentText()
{
    for (auto&[name, entity] : dataModel()->modelEntities()){
        if (entity->id() == value()){
            setCurrText(name);
        }
    }
}


void ForeignKeyField::cacheData()
{
}

std::size_t ForeignKeyField::cacheCount()
{
    return lookups[fieldName()]->count();
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
                if (std::get<0>(tup) == mDisplayField){
                    name = std::get<1>(tup)->displayName();
                    found = true;
                    break;
                }
            }
        }
    }

    return name;
}


DateField::DateField()
{
}

DateField::~DateField()
{
}

DateField::DateField(const std::string aName, const std::string aLabel)
    :Field(aName, aLabel)
    ,mValue{QDate(QDate::currentDate())}
{
    /*
    auto now = std::chrono::system_clock::now();
    auto today = date::floor<date::days>(now);
    date::year_month_day ymd = date::year_month_day(today);

    int y = int(ymd.year());
    int m = static_cast<int>(unsigned(ymd.month()));
    int d = static_cast<int>(unsigned(ymd.day()));

    mValue.setDate(y, m, d);
    */
}

std::string DateField::valueToString() const
{
    if (!mValue.isNull())
        return mValue.toString("yyyy-MM-dd").toStdString();
    else
        return "null";
}

std::string DateField::dbValueFormatter()
{
    return "'"+valueToString()+"'";
}

void DateField::setValue(QDate dt)
{
    mValue.setDate(dt.year(), dt.month(), dt.day());
}

void DateField::stringToValue(std::string val)
{
    QDate dt = QDate::fromString(stoq(val), "yyyy-MM-dd");
    setValue(dt);
}

QDate DateField::value()
{
    return mValue;
}

std::string DateField::displayName() const
{
    return valueToString();
}

DateTimeField::DateTimeField()
{
}

DateTimeField::~DateTimeField()
{
}

DateTimeField::DateTimeField(const std::string aName, const std::string aLabel)
    :Field(aName, aLabel)
    ,mValue{QDateTime()}
{
}

std::string DateTimeField::valueToString() const
{
    return mValue.toString("yyyy-MM-dd hh:mm").toStdString();
}

std::string DateTimeField::dbValueFormatter()
{
    return "'"+valueToString()+"'";
}

void DateTimeField::setValue(QDateTime dt)
{
    mValue.setDate(dt.date());
    mValue.setTime(dt.time());
}

void DateTimeField::stringToValue(std::string val)
{
    mValue.setDate(QDateTime::fromString(stoq(val)).date());
    mValue.setTime(QDateTime::fromString(stoq(val)).time());
}

QDateTime DateTimeField::value()
{
    return mValue;
}

std::string DateTimeField::displayName() const
{
    return valueToString();
}

TimeField::TimeField()
{
}

TimeField::TimeField(const std::string aName, const std::string aLabel)
    :Field(aName, aLabel)
    ,mValue{0,0}
{
}

TimeField::~TimeField()
{
}

std::string TimeField::valueToString() const
{
    return mValue.toString("hh:mm:ss").toStdString();
}

std::string TimeField::dbValueFormatter()
{
    return "'"+valueToString()+"'";
}

void TimeField::setValue(QTime tm)
{
    mValue.setHMS(tm.hour(), tm.minute(), tm.second());
}

void TimeField::stringToValue(std::string val)
{
    mValue = QTime::fromString(QString::fromStdString(val));
}

QTime TimeField::value()
{
    return mValue;
}

std::string TimeField::displayName() const
{
    return valueToString();
}
