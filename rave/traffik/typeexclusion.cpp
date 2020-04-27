#include <QStandardItem>
#include "typeexclusion.h"

TypeExclusion::TypeExclusion()
    :BaseEntity{}
    ,mName{}
    ,mDescription{}
    ,mDaypart1{}
    ,mDaypart2{}
    ,mDaypart3{}
    ,mDaypart4{}
    ,mDaypart5{}
    ,mDaypart6{}
    ,mDaypart7{}
{
    mName = createField<StringField>("exclusion_name", "Exclusion Name");
    mName->setDBColumnName("name");
    mName->setMandatory(true);

    mDescription = createField<TextField>("exclusion_desc", "Description");
    mDescription->setDBColumnName("description");

    mDaypart1 = createField<StringField>("daypart1", "Daypart1");
    mDaypart1->setSearchable(false);

    mDaypart2 = createField<StringField>("daypart2", "Daypart2");
    mDaypart2->setSearchable(false);

    mDaypart3 = createField<StringField>("daypart3", "Daypart3");
    mDaypart3->setSearchable(false);

    mDaypart4 = createField<StringField>("daypart4", "Daypart4");
    mDaypart4->setSearchable(false);
    mDaypart5 = createField<StringField>("daypart5", "Daypart5");
    mDaypart5->setSearchable(false);
    mDaypart6 = createField<StringField>("daypart6", "Daypart6");
    mDaypart6->setSearchable(false);
    mDaypart7 = createField<StringField>("daypart7", "Daypart6");
    mDaypart7->setSearchable(false);

    mHeader << QString::fromStdString(mName->fieldLabel())
            << QString::fromStdString(mDescription->fieldLabel());

    setTableName("rave_typeexclusion");

}

TypeExclusion::TypeExclusion(const TypeExclusion& other)
{
    if (other.mName)
        mName = other.mName;
    if (other.mDescription)
        mDescription = other.mDescription;
    if (other.mDaypart1)
        mDaypart1 = other.mDaypart1;
    if (other.mDaypart2)
        mDaypart2 = other.mDaypart2;
    if (other.mDaypart3)
        mDaypart3 = other.mDaypart3;
    if (other.mDaypart4)
        mDaypart4 = other.mDaypart4;
    if (other.mDaypart5)
        mDaypart5 = other.mDaypart5;
    if (other.mDaypart6)
        mDaypart6 = other.mDaypart6;
    if (other.mDaypart7)
        mDaypart7 = other.mDaypart7;
}

TypeExclusion& TypeExclusion::operator=(const TypeExclusion& other)
{
    if (this != &other){
        delete mName;
        delete mDescription;
        delete mDaypart1;
        delete mDaypart2;
        delete mDaypart3;
        delete mDaypart4;
        delete mDaypart5;
        delete mDaypart6;
        delete mDaypart7;

        mName = other.mName;
        mDescription = other.mDescription;
        mDaypart1 = other.mDaypart1;
        mDaypart2 = other.mDaypart2;
        mDaypart3 = other.mDaypart3;
        mDaypart4 = other.mDaypart4;
        mDaypart5 = other.mDaypart5;
        mDaypart6 = other.mDaypart6;
        mDaypart7 = other.mDaypart7;
    }
    return *this;
}

TypeExclusion::~TypeExclusion()
{
    /*
    delete mName;
    delete mDescription;
    delete mDaypart1;
    delete mDaypart2;
    delete mDaypart3;
    delete mDaypart4;
    delete mDaypart5;
    delete mDaypart6;
    delete mDaypart7;
    */
};

StringField* TypeExclusion::name() const
{
    return mName;
}

void TypeExclusion::setName(std::string aName)
{
    mName->setValue(aName);
}

TextField* TypeExclusion::description() const
{
    return mDescription;
}

void TypeExclusion::setDescription(std::string aDescription)
{
    mDescription->setValue(aDescription);
}

StringField* TypeExclusion::daypart1() const
{
    return mDaypart1;
}
void TypeExclusion::setDaypart1(std::string dp)
{
    mDaypart1->setValue(dp);
}

StringField* TypeExclusion::daypart2() const
{
    return mDaypart2;
}
void TypeExclusion::setDaypart2(std::string dp)
{
    mDaypart2->setValue(dp);
}

StringField* TypeExclusion::daypart3() const
{
    return mDaypart3;
}
void TypeExclusion::setDaypart3(std::string dp)
{
    mDaypart3->setValue(dp);
}

StringField* TypeExclusion::daypart4() const
{
    return mDaypart4;
}
void TypeExclusion::setDaypart4(std::string dp)
{
    mDaypart4->setValue(dp);
}

StringField* TypeExclusion::daypart5() const
{
    return mDaypart5;
}
void TypeExclusion::setDaypart5(std::string dp)
{
    mDaypart5->setValue(dp);
}

StringField* TypeExclusion::daypart6() const
{
    return mDaypart6;
}
void TypeExclusion::setDaypart6(std::string dp)
{
    mDaypart6->setValue(dp);
}

StringField* TypeExclusion::daypart7() const
{
    return mDaypart7;
}
void TypeExclusion::setDaypart7(std::string dp)
{
    mDaypart7->setValue(dp);
}

BaseEntity* TypeExclusion::copy() const
{
    return new TypeExclusion;
}
std::unique_ptr<BaseEntity> TypeExclusion::mapFields(StringMap* e)
{

    auto te = entityFieldMap<TypeExclusion>(e);
    return std::move(te);

    /*
    FieldValues fvs = mapping(e);
    std::unique_ptr<TypeExclusion> te = std::make_unique<TypeExclusion>();
    for (auto& fv : fvs)
        te->setValueByField(std::get<0>(fv), std::get<1>(fv));
    return std::move(te);
    */
}

QList<QStandardItem*> TypeExclusion::tableViewColumns()
{
    QString gname  = QString::fromStdString(name()->valueToString());
    QString desc = QString::fromStdString(description()->valueToString());

    QStandardItem* Qgname = new QStandardItem(gname);
    QStandardItem* Qdesc = new QStandardItem(desc);
    return{Qgname, Qdesc};
}
std::vector<std::string> TypeExclusion::tableViewValues()
{
    std::string gname  = name()->valueToString();
    std::string desc = description()->valueToString();
    return{gname, desc};
}
QStringList TypeExclusion::tableHeaders() const
{
    return mHeader;
}

std::string TypeExclusion::tableName() const
{
    return mTableName;
}
void TypeExclusion::setTableName(std::string table_name)
{
    mTableName = table_name;
}
std::string TypeExclusion::searchColumn() const
{
    return name()->valueToString();
}

void TypeExclusion::populateEntity()
{
    mName->setValueFromWidget();
    mDescription->setValueFromWidget();
}
