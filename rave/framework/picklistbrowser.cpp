#include "picklistbrowser.h"
#include "ui_picklistbrowser.h"

#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"
#include "selectclosewidget.h"

PickListBrowser::PickListBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent),
    ui(new Ui::PickListBrowser),
    scw{}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    scw = new SelectCloseWidget();
    bui->hlExtend->addWidget(scw);

}

PickListBrowser::~PickListBrowser()
{
    delete scw;
    delete ui;
}

void PickListBrowser::addRecord()
{
}
void PickListBrowser::updateRecord()
{
}
