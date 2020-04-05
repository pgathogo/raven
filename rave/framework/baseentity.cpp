#include "baseentity.h"

BaseEntity::~BaseEntity(){}

int BaseEntity::id() const
{
    return mID;
}

void BaseEntity::setId(int i)
{
    mID = i;
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
