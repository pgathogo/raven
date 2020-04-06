#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <memory>
#include <utility>
#include <tuple>

#include <QList>
#include "entityfields.h"

class QString;
class QStringList;

class QStandardItem;

//using FieldMap = std::map<std::string, std::unique_ptr<Field>>;
using FieldMap = std::tuple<std::string, std::unique_ptr<Field>>;
using StringMap = std::map<std::string, std::string>;

class BaseEntity
{
public:
    BaseEntity():mID{-1}{}
    virtual ~BaseEntity();
    int id() const;
    void setId(int i);
    virtual BaseEntity* copy()const = 0;
    virtual BaseEntity* mapFields(StringMap* e) = 0;

    std::vector<FieldMap>::const_iterator cBeginIter();
    std::vector<FieldMap>::const_iterator cEndIter();

    std::vector<FieldMap>::iterator beginIter();
    std::vector<FieldMap>::iterator endIter();

    virtual QList<QStandardItem*> cols() = 0;

    virtual QStringList tableHeaders() const = 0;

    virtual std::string tableName() const = 0;

    std::vector<std::string> dbColumnNames();

    size_t fieldsCount(){ return mFields.size(); }

    template<typename T, typename... TArgs>
    T* createField(TArgs... mArgs){
        static_assert(std::is_base_of<Field, T>::value, "`T` must be derived from Field");
        auto uPtr = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
        FieldMap fm;
        auto ptr(uPtr.get());
        fm = std::make_tuple(uPtr->fieldName(), std::move(uPtr));
        mFields.emplace_back(std::move(fm));
        return ptr;
    }

    void setValueByField(Field* fld, std::string val)
    {

        std::vector<FieldMap>::iterator iter;
        for (iter=beginIter(); iter != endIter(); ++iter){
            if (std::get<0>(*iter) == fld->fieldName())
               std::get<1>(*iter)->stringToValue(val);
        }

    }

    /*
    template<typename T>
    typename T::type dbValue(T* fld)
    {
        typename T::type val = std::get<typename T::type>(fld->getValue());
        return std::get<typename T::type>(fld->dbValueFormatter(val));
    }


    template<typename T>
    std::string dbValueAsString(T* fld)
    {
        typename T::type val = std::get<typename T::type>(fld->getValue());
        return fld->dbValueFormatter(val);
    }
  */

private:
    int mID;
    std::vector<FieldMap> mFields;

};

#endif // BASEENTITY_H
