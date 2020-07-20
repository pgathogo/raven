#include <QDebug>
#include <QList>
#include <QStandardItem>

#include "valuelist.h"

ValueList::ValueList()
        :BaseEntity()
        ,mCode{}
        ,mListValue{}
{
    qDebug() << "ValueList::ctor" ;

    mCode = createField<StringField>("code", "Code");
    mListValue = createField<StringField>("value", "Value");
    mListValue->setMandatory(true);

    mHeader << QString::fromStdString(mListValue->fieldLabel())
            << QString::fromStdString(mCode->fieldLabel());
}


ValueList::ValueList(const ValueList& rhs)
{
    qDebug() << "ValueList::copy ctor";
    if (rhs.mCode)
        *mCode = *rhs.mCode;
    if (rhs.mListValue)
        *mListValue = *rhs.mListValue;
    mHeader = rhs.mHeader;
    mTableName = rhs.tableName(); //mTableName;
}

ValueList& ValueList::operator=(const ValueList& rhs)
{
    qDebug() << "ValueList::operator=";

    if (this != &rhs){
        //delete mCode;
        //delete mListValue;

        //mCode = createField<StringField>(rhs.mCode->, "Code");
        //mListValue = createField<StringField>("value", "Value");
        //mListValue->setMandatory(true);

        *mCode = *rhs.mCode;
        *mListValue = *rhs.mListValue;

        mHeader = rhs.mHeader;
        mTableName = rhs.tableName(); //mTableName;
    }

    return *this;
}


ValueList::~ValueList()
{
    qDebug() << "ValueList::dtor";
}


StringField* ValueList::code() const
{
    return mCode;
}


void ValueList::setCode(std::string pCode)
{
    mCode->setValue(pCode);
}


StringField* ValueList::listValue() const
{
    return mListValue;
}


void ValueList::setListValue(std::string pValue)
{
    mListValue->setValue(pValue);
}

std::unique_ptr<BaseEntity> ValueList::mapFields(StringMap* sm)
{
    auto te = entityFieldMap<ValueList>(sm);
    return std::move(te);
}


std::list<std::string> ValueList::tableViewColumns() const
{
    std::list<std::string> cols;

    cols.push_back(listValue()->valueToString());
    cols.push_back(code()->valueToString());

    return cols;

}

std::vector<std::string> ValueList::tableViewValues()
{
    std::string vValue = listValue()->valueToString();
    std::string vCode = code()->valueToString();
    return {vValue, vCode};
}

QStringList ValueList::tableHeaders() const
{
    return mHeader;
}

void ValueList::setTableName(std::string table_name)
{
    mTableName = table_name;
}

std::string ValueList::tableName() const
{
    return mTableName;
}

std::string ValueList::valueListTableName() const
{
    return "";
}

std::string ValueList::searchColumn() const
{
    return listValue()->valueToString();
}

void ValueList::populateEntity()
{
    //mCode->setValueFromWidget();
    //mListValue->setValueFromWidget();
}

std::unique_ptr<BaseEntity> ValueList::cloneAsUnique()
{
    return std::make_unique<ValueList>();
}

void ValueList::afterMapping(BaseEntity &entity)
{

}

std::string ValueList::windowTitle() const
{
    return "";
}

/* ----- Gender ------- */


std::string Gender::valueListTableName() const
{
    return "rave_gender";
}

std::string Gender::windowTitle() const
{
    return "Gender Lookup";
}

std::string Gender::tableName() const
{
    return "rave_gender";
}

