#include <QDebug>
#include "picklistbrowser.h"
#include "ui_picklistbrowser.h"

#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"
#include "selectclosewidget.h"
#include "manytomany.h"

PickListBrowser::PickListBrowser(ManyToMany& mtom,
                                EntityDataModel* edm,
                                 QWidget *parent) :
    BaseEntityBrowserDlg(parent, mtom.detailEntity()),
    ui(new Ui::PickListBrowser),
    scw{},
    mEntity{mtom.detailEntity()},
    destModel{edm},
    mMtoM{mtom}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    scw = new SelectCloseWidget();

    connect(scw->selectBtn(), &QPushButton::clicked, this, &PickListBrowser::onSelectItem);
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

void PickListBrowser::onSelectItem()
{
    qDebug() << "PickListBrowser::onSelectItem()";
    std::string searchName = selectedRowName().toStdString();
    BaseEntity* entity = entityDataModel()->findEntityByName(searchName);
    destModel->cacheEntity(entity);
    mMtoM.addEntity(entity);

   // BaseEntity& ent = *entity;
    //BaseEntity* evil = &ent;
}

