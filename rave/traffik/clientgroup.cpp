#include <typeinfo>

#include <typeinfo>
#include <QDebug>
#include <QList>
#include <QStandardItem>

#include "clientgroup.h"

ClientGroup::ClientGroup()
    :BaseEntity(),
     mName{nullptr},
     mDescription{nullptr}
{
    mName = createField<StringField>("group_name","Group Name");
    mName->setDBColumnName("name");
    mDescription = createField<TextField>("group_description","Group Desc.");
    mDescription->setDBColumnName("description");

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mDescription->fieldLabel());

}

ClientGroup::~ClientGroup()
{
    qDebug() << "ClientGroup::dtor";
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


BaseEntity* ClientGroup::mapFields(StringMap* e)
{
   std::map<std::string, std::string>::const_iterator it;
   std::vector<FieldMap>::iterator iter;

   ClientGroup* ct = new ClientGroup;

   for(it=e->cbegin(); it != e->cend(); ++it){

       for(iter=beginIter(); iter != endIter(); ++iter){

           if ((std::get<1>(*iter)->dbColumnName() == it->first) &&
               (std::get<1>(*iter)->visible())){
                auto ptr(std::get<1>(*iter).get());
                ct->setValueByField(ptr, it->second);
           }

       }

    }

   return ct;
}

QList<QStandardItem*> ClientGroup::cols()
{
    QString gname  = QString::fromStdString(name()->valueToString());
    QString desc = QString::fromStdString(description()->valueToString());

    QStandardItem* Qgname = new QStandardItem(gname);
    QStandardItem* Qdesc = new QStandardItem(desc);
    return{Qgname, Qdesc};
}

QStringList ClientGroup::tableHeaders() const
{
    return mHeader;
}


std::string ClientGroup::tableName() const
{
    return "rave_clientgroup";
}

std::string ClientGroup::searchColumn() const
{
    return name()->valueToString();
}
