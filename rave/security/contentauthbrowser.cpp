#include "contentauthbrowser.h"
#include "ui_contentauthbrowser.h"

#include "content.h"
#include "contentauth.h"
#include "contentauthform.h"
#include "../framework/baseentity.h"
#include "../framework/entityregister.h"

ContentAuthBrowser::ContentAuthBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<ContentAuth>()),
    ui(new Ui::ContentAuthBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Content Authorization");
}

ContentAuthBrowser::~ContentAuthBrowser()
{
    delete ui;
}

void ContentAuthBrowser::addRecord()
{
    auto contauth = std::make_unique<ContentAuth>();

    auto contForm = std::make_unique<ContentAuthForm>(
                contauth.get());

    if (contForm->exec() > 0){

        try{
            // Grant or Revoke neccessary tables at this point

            grant_revoke_content_access(contauth.get());

            entityDataModel().createEntity(std::move(contauth));

        } catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void ContentAuthBrowser::updateRecord()
{

    int entity_id = id_from_name(selectedRowName().toStdString());

    std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(std::to_string(entity_id));
    ContentAuth* contAuth = dynamic_cast<ContentAuth*>(be.get());
    std::unique_ptr<ContentAuthForm> authForm =
            std::make_unique<ContentAuthForm>(contAuth, this);

    if (authForm->exec() > 0){
        try{

        grant_revoke_content_access(contAuth);

        entityDataModel().updateEntity(*contAuth);
        entityDataModel().all();

        }catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }


}

void ContentAuthBrowser::deleteRecord()
{
    int entity_id = id_from_name(selectedRowName().toStdString());

    BaseEntity* be =  nullptr;

    bool found = false;
    for(auto&[entity_name, entity] : entityDataModel().modelEntities()){
        if (found) break;
        for (auto&[fld_name, field] : entity->fields() ) {
            if (entity->searchColumn() == std::to_string(entity_id)){
                be = entity.get();
                be->setDBAction(DBAction::dbaDELETE);
                found = true;
                break;
            }
        }
    }

    if (be){

        try{
            entityDataModel().deleteEntity(*be);
            removeSelectedRow();
        } catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }


}

void ContentAuthBrowser::grant_revoke_content_access(ContentAuth* contAuth)
{
    Content* cont = dynamic_cast<Content*>(contAuth->content()->currentEntity());
    std::string table_name =cont->contentTableName()->value();

    std::string role_name = contAuth->role()->currText();

    QString str = "%1 %2 on TABLE %3 %4 %5;";
    // <Grant> <SELECT> on TABLE <table_name> <TO> <role_name>;

    auto stmt_tokens = [](int bit, const std::string priv){
        return (bit) ? std::make_tuple("GRANT", priv, "TO")
                     : std::make_tuple("REVOKE", priv, "FROM"); };

    using SS = std::string;
    auto priv_stmt = [&table_name, &role_name, &str](std::tuple<SS,SS,SS> tokens){
        auto[t1, t2, t3] = tokens;
        return str.arg(stoq(t1), stoq(t2), stoq(table_name),
                stoq(t3), stoq(role_name)).toStdString(); };

    std::string create = priv_stmt(stmt_tokens(
               contAuth->createBit()->value(),
                "INSERT"));

    std::string read = priv_stmt(stmt_tokens(
                contAuth->readBit()->value(),
                "SELECT"));

    std::string update = priv_stmt(stmt_tokens(
                contAuth->updateBit()->value(),
                "UPDATE"));

    std::string del = priv_stmt(stmt_tokens(
                contAuth->deleteBit()->value(),
                "DELETE"));

    std::string sql = create+read+update+del;

    EntityDataModel edm;
    edm.executeRawSQL(sql);

}

int ContentAuthBrowser::id_from_name(const std::string search_name)
{
    //std::string searchName = selectedRowName().toStdString();

    int entity_id = -1;

    if (!search_name.empty()){
        // extract db column name from the search field in
        // the combobox
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();

        auto cont_auth = std::make_unique<ContentAuth>();
        bool found = false;
        // Find a field in ContentAuth entity with the same db column name.
        for (auto&[name, field] : cont_auth->fields()){
            if (found) break;
            if (field->dbColumnName() == columnName){
                Field* f = field.get();
                auto fkfield = static_cast<ForeignKeyField*>(f);
                // Find and extract Id value of an entity in the
                // ForeignKey data model attached to the current field.
                for(auto& [fname, entity] : fkfield->dataModel()->modelEntities()){
                    if (fname == search_name){
                        entity_id = entity->id();
                        found = true;
                        break;
                    }
                }
            }
        }
    }

        return entity_id;
}
