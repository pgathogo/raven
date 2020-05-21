#include <typeinfo>
#include <QDebug>
#include <QList>
#include <QStandardItem>

#include "clientgroup.h"

ClientGroup::ClientGroup()
    :BaseEntity(),
     mName{},
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
    qDebug() << "ClientGroup::dtor";
    //delete mName;
    //delete mDescription;
}

BaseEntity* ClientGroup::copy() const
{
    qDebug() << "ClientGroup copy ...";
    return new ClientGroup();
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


QList<QStandardItem*> ClientGroup::tableViewColumns()
{
    QString gname  = QString::fromStdString(name()->valueToString());
    QString desc = QString::fromStdString(description()->valueToString());

    QStandardItem* Qgname = new QStandardItem(gname);
    QStandardItem* Qdesc = new QStandardItem(desc);
    return{Qgname, Qdesc};
}

std::vector<std::string> ClientGroup::tableViewValues()
{
    std::string gname  = name()->valueToString();
    std::string desc = description()->valueToString();
    return{gname, desc};
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
    mName->setValueFromWidget();
    mDescription->setValueFromWidget();
}
