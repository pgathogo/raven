#include <QDebug>
#include "baseentity.h"
#include "entitydatamodel.h"
#include "../utils/types.h"

BaseEntity::BaseEntity()
        :mID{}
        ,mEDM{nullptr}
        ,mDBAction{DBAction::dbaNONE}
{
    mID = createField<IntegerField>("id", "Unique identifier");
    mID->setSearchable(false);
}

BaseEntity::~BaseEntity()
{
    qDebug() << "BaseEntity::dtor";
}

int BaseEntity::id() const
{
    return mID->value();
}

void BaseEntity::setId(int i)
{
    mID->setValue(i);
}

std::vector<FieldMap>::const_iterator BaseEntity::cBeginIter()
{
    return mFields.cbegin();
}

std::vector<FieldMap>::const_iterator BaseEntity::cEndIter()
{
    return mFields.cend();
}

std::vector<FieldMap>::iterator BaseEntity::beginIter()
{
    return mFields.begin();
}

std::vector<FieldMap>::iterator BaseEntity::endIter()
{
    return mFields.end();
}


std::vector<std::string> BaseEntity::dbColumnNames()
{
    // All fields are database columns, unless they are marked
    // formOnly.
    std::vector<std::string> cols;
    auto& flds = fields();
    for (auto& f : flds){
        if (!std::get<1>(f)->formOnly())
            cols.push_back(std::get<1>(f)->dbColumnName());
    }

    return cols;
}

ActionResult BaseEntity::validate()
{
    // Fields marked as mandatory should have values
    ActionResult ar = std::make_tuple(ActionResultType::arSUCCESS,"");
    auto cIter = cBeginIter();
    for(; cIter != cEndIter(); ++cIter){
        if (std::get<1>(*cIter)->mandatory()){
            if (std::get<1>(*cIter)->valueToString().empty()){
                ar = std::make_tuple(ActionResultType::arERROR,
                                     "`"+std::get<1>(*cIter)->fieldLabel()+"` has no value!");
                break;
            }
        }
    }

    return ar;
}

void BaseEntity::setValueByField(Field* fld, const std::string& val)
{
    std::vector<FieldMap>::iterator iter;
    for (iter=beginIter(); iter != endIter(); ++iter){
        if (std::get<0>(*iter) == fld->fieldName())
           std::get<1>(*iter)->stringToValue(val);
    }

}

FieldValues BaseEntity::mapping(StringMap* e)
{
    std::map<std::string, std::string>::const_iterator it;
    std::vector<FieldMap>::iterator iter;
    FieldValues flds;

    std::tuple<Field*, std::string> fieldVal;

    for(it=e->cbegin(); it != e->cend(); ++it){

       for(iter=beginIter(); iter != endIter(); ++iter){

           if ((std::get<1>(*iter)->dbColumnName() == it->first) &&
           (std::get<1>(*iter)->visible())){

               std::string col = std::get<1>(*iter)->dbColumnName();
               std::string val = it->second;

            Field* ptr(std::get<1>(*iter).get());
            fieldVal = std::make_tuple(ptr, it->second);
            flds.push_back(fieldVal);
           }

         }

      }

       return flds;
}

void BaseEntity::getEntityById(std::unique_ptr<BaseEntity> entity, int id)
{

    mEDM = std::make_unique<EntityDataModel>(std::move(entity));
    mEDM->getById({"id", id});

}

void BaseEntity::setDBAction(DBAction dbact)
{
    mDBAction = dbact;
}

DBAction BaseEntity::dbAction() const
{
    return mDBAction;
}
void BaseEntity::clearFields()
{
    mFields.clear();
}

std::vector<FieldMap> const& BaseEntity::fields()
{
    return mFields;
}

void BaseEntity::baseMapFields(StringMap* map)
{
    FieldValues fval = mapping(map);
    for(auto& v : fval)
        this->setValueByField(std::get<0>(v), std::get<1>(v));
}
