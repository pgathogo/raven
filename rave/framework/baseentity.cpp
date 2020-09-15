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

BaseEntity::BaseEntity(Key)
    :mID{}
    ,mEDM{nullptr}
    ,mDBAction{DBAction::dbaNONE}
{
    mID = createField<IntegerField>("id", "Unique identifier");
    mID->setSearchable(false);
}

BaseEntity::~BaseEntity()
{
}

int BaseEntity::id() const
{
    return mID->value();
}

void BaseEntity::setId(int i)
{
    mID->setValue(i);
}

IntegerField& BaseEntity::getId()
{
    return *mID;
}

IntegerField* BaseEntity::uniqueId() const
{
    return mID;
}

std::vector<std::string> BaseEntity::dbColumnNames() const
{
    // All fields are database columns, unless they are marked
    // formOnly.
    std::vector<std::string> cols;

    for (auto& [name, field] : fields()){
        if ( !field->formOnly())
            cols.push_back(field->dbColumnName());
    }

    return cols;
}

ActionResult BaseEntity::validate()
{
    // Fields marked as mandatory should have values
    ActionResult ar = std::make_tuple(ActionResultType::arSUCCESS,"");

    for (auto& [name, field] : fields()){
        if (field->mandatory()){
            if (field->valueToString().empty()){
                ar = std::make_tuple(ActionResultType::arERROR,
                                     "`"+field->fieldLabel()+"` has no value!");
                break;
            }
        }
    }

    return ar;
}

void BaseEntity::setValueByField(const Field& fld, const std::string& val)
{
    for (auto& [name, field] : fields()){
        if (name == fld.fieldName())
           field->stringToValue(val);
    }

}

FieldValues BaseEntity::mapping(StringMap* e)
{
    std::map<std::string, std::string>::const_iterator it;
    FieldValues mapped_fields;

    std::tuple<Field*, std::string> fieldVal;

    for(it=e->cbegin(); it != e->cend(); ++it){

        for (auto& [name, field] : fields()){

           if ( (field->dbColumnName() == it->first) && (field->visible()) ){

               std::string col = field->dbColumnName();
               std::string val = it->second;

            Field* ptr(field.get());
            fieldVal = std::make_tuple(ptr, it->second);
            mapped_fields.push_back(fieldVal);
           }

         }

      }

    return mapped_fields;
}

void BaseEntity::populateEntity()
{

}

void BaseEntity::getEntityById(std::unique_ptr<BaseEntity> entity, int id)
{

    // To be refactored
    std::string id_column = entity->uniqueId()->dbColumnName();
    mEDM = std::make_unique<EntityDataModel>(std::move(entity));
    mEDM->getById({id_column, id});

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

std::vector<FieldMap> const& BaseEntity::fields() const
{
    return mFields;
}

std::string BaseEntity::displayName()
{
    return "BaseEntity";
}

std::string BaseEntity::make_create_stmt()
{
   return "";
}

std::string BaseEntity::make_alter_stmt(const std::string name)
{
    return "";
}

std::string BaseEntity::make_drop_stmt(const std::string name)
{
    return "";
}

std::string BaseEntity::className()
{
    return  demangle(typeid(this).name());
}

void BaseEntity::baseMapFields(StringMap* map)
{
    for(auto& [field, name] : mapping(map))
        this->setValueByField(*field, name);
}

bool BaseEntity::isNew()
{
    return (id() == -1) ? true : false;
}
