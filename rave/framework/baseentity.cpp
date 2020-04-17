#include "baseentity.h"

BaseEntity::BaseEntity()
        :mID{nullptr}
{
    mID = createField<IntegerField>("id", "Unique identifier");
    mID->setSearchable(false);
}

BaseEntity::~BaseEntity()
{
    //delete mID;
    //mFields.clear();
}

int BaseEntity::id() const
{
    return mID->value().toInt();
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
    std::vector<std::string> cols;
    std::vector<FieldMap>::const_iterator citer;
    for (citer=cBeginIter(); citer != cEndIter(); ++citer){
        if (!std::get<1>(*citer)->formOnly())
            cols.push_back(std::get<1>(*citer)->dbColumnName());
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
            Field* ptr(std::get<1>(*iter).get());
            fieldVal = std::make_tuple(ptr, it->second);
            flds.push_back(fieldVal);
           }

         }

      }

       return flds;
}
