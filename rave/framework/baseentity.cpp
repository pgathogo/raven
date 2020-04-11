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

ErrorMessage BaseEntity::validate()
{
    // Fields marked as mandatory should have values
    ErrorMessage em = std::make_tuple(true,"");
    auto cIter = cBeginIter();
    for(; cIter != cEndIter(); ++cIter){
        if (std::get<1>(*cIter)->mandatory()){
            if (std::get<1>(*cIter)->valueToString().empty()){
                em = std::make_tuple(false,
                                     "`"+std::get<1>(*cIter)->fieldLabel()+"` has no value!");
                break;
            }
        }
    }

    return em;
}
