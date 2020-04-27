#include <QList>
#include <QStandardItem>

#include "valuelist.h"

ValueList::ValueList()
        :BaseEntity()
        ,mCode{}
        ,mListValue{}
{
    mCode = createField<StringField>("code", "Code");
    mListValue = createField<StringField>("value", "Value");
    mListValue->setMandatory(true);

    mHeader << QString::fromStdString(mListValue->fieldLabel())
            << QString::fromStdString(mCode->fieldLabel());
}

ValueList::ValueList(const ValueList& vl)
{
    if (vl.mCode)
        mCode = vl.mCode;
    if (vl.mListValue)
        mListValue = vl.mListValue;
}

ValueList& ValueList::operator=(const ValueList& vl)
{
    if (this != &vl){
        delete mCode;
        delete mListValue;

        mCode = vl.mCode;
        mListValue = vl.mListValue;
    }
    return *this;
}

ValueList::~ValueList()
{
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


BaseEntity* ValueList::copy() const
{
    return new ValueList();
}


std::unique_ptr<BaseEntity> ValueList::mapFields(StringMap* sm)
{
    FieldValues fvs = mapping(sm);
    std::unique_ptr<ValueList> vl = std::make_unique<ValueList>();
    for (auto& fv : fvs)
        vl->setValueByField(std::get<0>(fv), std::get<1>(fv));
        vl->setTableName(this->tableName());
    return std::move(vl);
}


QList<QStandardItem*> ValueList::tableViewColumns()
{
    QString tvCode = QString::fromStdString(code()->valueToString());
    QString tvValue = QString::fromStdString(listValue()->valueToString());

    QStandardItem* qCode = new QStandardItem(tvCode);
    QStandardItem* qValue = new QStandardItem(tvValue);

    return {qValue, qCode};
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
    mCode->setValueFromWidget();
    mListValue->setValueFromWidget();
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
