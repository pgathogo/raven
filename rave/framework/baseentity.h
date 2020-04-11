#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <memory>
#include <utility>
#include <tuple>

#include <QList>
#include "entityfields.h"

#include "../utils/types.h"

class QString;
class QStringList;

class QStandardItem;

//using FieldMap = std::map<std::string, std::unique_ptr<Field>>;
using FieldMap = std::tuple<std::string, std::unique_ptr<Field>>;
using StringMap = std::map<std::string, std::string>;
using FieldValue = std::tuple<Field*, std::string>;
using FieldValues = std::vector<std::tuple<Field*, std::string>>;

class BaseEntity
{
public:
    BaseEntity();
    virtual ~BaseEntity();

    [[nodiscard]] int id() const;
    void setId(int i);

    virtual BaseEntity* copy()const = 0;
    virtual std::unique_ptr<BaseEntity> mapFields(StringMap* e) = 0;

    std::vector<FieldMap>::const_iterator cBeginIter();
    std::vector<FieldMap>::const_iterator cEndIter();

    std::vector<FieldMap>::iterator beginIter();
    std::vector<FieldMap>::iterator endIter();

    virtual QList<QStandardItem*> tableViewColumns() = 0;
    virtual std::vector<std::string> tableViewValues() = 0;

    virtual QStringList tableHeaders() const = 0;

    virtual std::string tableName() const = 0;

    std::vector<std::string> dbColumnNames();

    size_t fieldsCount(){ return mFields.size(); }

    virtual std::string searchColumn() const = 0;

    [[nodiscard]] virtual ErrorMessage validate();

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

    void setValueByField(Field* fld, const std::string& val)
    {
        std::vector<FieldMap>::iterator iter;
        for (iter=beginIter(); iter != endIter(); ++iter){
            if (std::get<0>(*iter) == fld->fieldName())
               std::get<1>(*iter)->stringToValue(val);
        }

    }

    FieldValues mapping(StringMap* e)
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


private:
    IntegerField* mID;
    std::vector<FieldMap> mFields;

};

#endif // BASEENTITY_H
