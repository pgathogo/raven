#include "contentbrowser.h"
#include "ui_contentbrowser.h"
#include "../framework/ui_baseentitybrowserdlg.h"

#include "content.h"
#include "contentform.h"
#include "contentauth.h"

ContentBrowser::ContentBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<Content>()),
    ui(new Ui::ContentBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Application Contents");
    hideEditButton();
    searchRecord();
}

ContentBrowser::~ContentBrowser()
{
    delete ui;
}

void ContentBrowser::addRecord()
{
    //auto edm = std::make_unique<EntityDataModel>();

    auto content = std::make_unique<Content>();

    std::unique_ptr<ContentForm> contentForm =
            std::make_unique<ContentForm>(content.get(),
                                          entityDataModel());

    if (contentForm->exec() > 0){
        for (auto& [name, entity] : entityDataModel().modelEntities()){
            Content* cont = dynamic_cast<Content*>(entity.get());
            if (cont->dbAction() == DBAction::dbaCREATE){
                entityDataModel().createEntityDB(*cont);
            }
            if (cont->dbAction() == DBAction::dbaDELETE){
                entityDataModel().deleteEntity(*cont);
                removeSelectedRow();
            }
        }

    }

}

void ContentBrowser::updateRecord()
{

}

void ContentBrowser::deleteRecord()
{
    // Check if content is been used for authorization
    BaseEntity* entity = findSelectedEntity();

    if (roles_attached(entity)){
        showMessage("Roles attached to this content!");
        return;
    }

    BaseEntityBrowserDlg::deleteRecord();

}

bool ContentBrowser::roles_attached(const BaseEntity *entity)
{
    auto cauth = std::make_unique<ContentAuth>();
    std::string fname = cauth->content()->dbColumnName();
    EntityDataModel edm(std::move(cauth));
    auto auth_filter = std::make_tuple(fname, "=", entity->id());
    std::string filter = edm.prepareFilter(auth_filter);
    edm.search(filter);

    return (edm.count() > 0 ) ? true : false;
}

/*
void ContentBrowser::searchRecord()
{

}
*/
