#include <typeinfo>
#include <QDebug>

#include "clientgroup.h"
#include "../framework/choicefield.h"

ClientGroup::ClientGroup()
     :mName{},
     mDescription{}
{
    mName = createField<StringField>("group_name","Group Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);
    mDescription = createField<TextField>("group_description","Group Desc.");
    mDescription->setDBColumnName("description");

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mDescription->fieldLabel());

    setTableName("rave_clientgroup");
}

ClientGroup::ClientGroup(int)
{
    setTableName("rave_clientgroup");
}

ClientGroup::ClientGroup(const ClientGroup& other)
{
    qDebug() << "ClientGroup::copy ctor";

    if (other.mName)
        mName = other.mName;
    if (other.mDescription)
        mDescription = other.mDescription;
}

ClientGroup& ClientGroup::operator=(const ClientGroup& other)
{
    qDebug() << "ClientGroup::assignment operator";

    if (this != &other){
        delete mName;
        delete mDescription;

        mName = other.mName;
        mDescription = other.mDescription;
    }
    return *this;
}


ClientGroup::~ClientGroup()
{
}

void ClientGroup::setName(std::string aName)
{
    mName->setValue(aName);
}

void ClientGroup::setDescription(std::string aDescription)
{
    mDescription->setValue(aDescription);
}


StringField* ClientGroup::name() const
{
    return mName;
}

TextField* ClientGroup::description() const
{
    return mDescription;
}

std::unique_ptr<BaseEntity> ClientGroup::mapFields(StringMap* sm)
{
    auto cg = entityFieldMap<ClientGroup>(sm);
    return std::move(cg);

}


std::vector<std::string> ClientGroup::tableViewColumns() const
{
    return tableViewCols<std::string>(
                name()->displayName(),
                description()->displayName()
                );
}

std::vector<std::string> ClientGroup::tableViewValues()
{
    return tableViewCols<std::string>(
                name()->valueToString(),
                description()->valueToString()
                );
}

QStringList ClientGroup::tableHeaders() const
{
    return mHeader;
}

void ClientGroup::setTableName(std::string table_name)
{
    mTableName = table_name;
}

std::string ClientGroup::tableName() const
{
    return mTableName;
}

std::string ClientGroup::searchColumn() const
{
    return name()->valueToString();
}

void ClientGroup::populateEntity()
{
    //mName->setValueFromWidget();
    //mDescription->setValueFromWidget();
}

std::shared_ptr<BaseEntity> ClientGroup::cloneAsShared()
{
    return std::make_shared<ClientGroup>();
}

void ClientGroup::afterMapping(BaseEntity &entity)
{

}

/*
std::string ClientGroup::className()
{
    return  rave_demangler(typeid(this).name());
}
*/
