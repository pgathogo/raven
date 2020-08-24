#include "contentbrowser.h"
#include "ui_contentbrowser.h"
#include "ui_baseentitybrowserdlg.h"

#include "content.h"
#include "contentform.h"

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
        }

    }

}

void ContentBrowser::updateRecord()
{

}

/*
void ContentBrowser::searchRecord()
{

}
*/
