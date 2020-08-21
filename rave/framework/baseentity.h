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

class EntityDataModel;

using FieldMap = std::tuple<std::string, std::unique_ptr<Field>>;
using StringMap = std::map<std::string, std::string>;
//using FieldValue = std::tuple<Field*, std::string>;
using FieldValues = std::vector<std::tuple<Field*, std::string>>;


class BaseEntity
{
public:
    BaseEntity();
    virtual ~BaseEntity();

    [[nodiscard]] virtual int id() const;
    void setId(int i);
    IntegerField& getId();

    virtual IntegerField* uniqueId() const;

    virtual std::unique_ptr<BaseEntity> mapFields(StringMap* e) = 0;

    virtual std::list<std::string> tableViewColumns() const = 0;
    virtual std::vector<std::string> tableViewValues() = 0;

    virtual QStringList tableHeaders() const = 0;

    virtual std::string tableName() const = 0;
    virtual void setTableName(const std::string table_name)=0;

    std::vector<std::string> dbColumnNames() const;

    size_t fieldsCount() const { return mFields.size(); }

    virtual std::string searchColumn() const = 0;

    [[nodiscard]] virtual ActionResult validate();

    void setValueByField(const Field& fld, const std::string& val);
    FieldValues mapping(StringMap* e);

    virtual void populateEntity()=0;

    //virtual std::string classDisplayName() const;

    void getEntityById(std::unique_ptr<BaseEntity> entity, int id);

    void setDBAction(DBAction dbact);
    DBAction dbAction() const;

    void clearFields();

    std::vector<FieldMap> const& fields() const;

    virtual std::unique_ptr<BaseEntity> cloneAsUnique() = 0;

    virtual void afterMapping(BaseEntity& entity) = 0;

    virtual std::string displayName();
    virtual std::string make_create_stmt();
    virtual std::string make_alter_stmt(const std::string name);
    virtual std::string make_drop_stmt(const std::string name);

    template<typename T>
    std::unique_ptr<T> clone()
    {
        return std::make_unique<T>();
    }

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

    template<typename T, typename... TArgs>
    std::unique_ptr<T> entityFieldMap(StringMap* map, TArgs...mArgs)
    {
        std::unique_ptr<T> uT = std::make_unique<T>(std::forward<TArgs>(mArgs)...);

        for(auto& [field, name] : mapping(map))
            uT->setValueByField(*field, name);

        return std::move(uT); // Evil !! - transfering ownership from local
    }

    void baseMapFields(StringMap* map);

private:
    IntegerField* mID;
    std::vector<FieldMap> mFields;
    std::unique_ptr<EntityDataModel> mEDM;
    DBAction mDBAction;

};

#endif // BASEENTITY_H
