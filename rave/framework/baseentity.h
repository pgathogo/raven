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
    virtual void setTableName(std::string table_name)=0;

    std::vector<std::string> dbColumnNames();

    size_t fieldsCount(){ return mFields.size(); }

    virtual std::string searchColumn() const = 0;

    [[nodiscard]] virtual ActionResult validate();

    void setValueByField(Field* fld, const std::string& val);
    FieldValues mapping(StringMap* e);

    virtual void populateEntity()=0;

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

private:
    IntegerField* mID;
    std::vector<FieldMap> mFields;

};

#endif // BASEENTITY_H
