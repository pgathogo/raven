#include "contentauthbrowser.h"
#include "ui_contentauthbrowser.h"

#include "contentauth.h"
#include "contentauthform.h"

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

            entityDataModel().createEntity(std::move(contauth));

        } catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void ContentAuthBrowser::updateRecord()
{
    std::string searchName = selectedRowName().toStdString();

    if (!searchName.empty()){
        // extract db column name from the search field in
        // the combobox
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();

        auto cont_auth = std::make_unique<ContentAuth>();
        bool found = false;
        int entity_id = -1;
        // Find a field in ContentAuth entity with the same db column name.
        for (auto&[name, field] : cont_auth->fields()){
            if (found) break;
            if (field->dbColumnName() == columnName){
                Field* f = field.get();
                auto fkfield = static_cast<ForeignKeyField*>(f);
                // Find and extract Id value of an entity in the
                // ForeignKey data model attached to the curren field.
                for(auto& [fname, entity] : fkfield->dataModel()->modelEntities()){
                    if (fname == searchName){
                        entity_id = entity->id();
                        found = true;
                        break;
                    }
                }
            }
        }

        BaseEntity* be = entityDataModel().findEntityByName(std::to_string(entity_id));
        ContentAuth* contAuth = dynamic_cast<ContentAuth*>(be);
        std::unique_ptr<ContentAuthForm> authForm =
                std::make_unique<ContentAuthForm>(contAuth, this);

        if (authForm->exec() > 0){
            try{
                entityDataModel().updateEntity(*contAuth);
            }catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }

    }

}
