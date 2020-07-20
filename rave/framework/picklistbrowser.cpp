#include <QDebug>
#include "picklistbrowser.h"
#include "ui_picklistbrowser.h"

#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"
#include "selectclosewidget.h"
#include "manytomany.h"

PickListSetting::PickListSetting(std::unique_ptr<BaseEntity> entity,
                                 PickListMode plm)
    :listEntity{std::move(entity)}
    ,pickMode{plm}

{
}

PickListBrowser::PickListBrowser(PickListSetting& plSetting,
                                 QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::move(plSetting.listEntity)),
    ui(new Ui::PickListBrowser),
    scw{},
    mPickListSetting{plSetting}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    scw = new SelectCloseWidget();

    connect(scw->selectBtn(), &QPushButton::clicked, this, &PickListBrowser::onSelectItem);
    connect(scw->closeBtn(), &QPushButton::clicked, this, &PickListBrowser::onCloseSelection);

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
    // get a stored pointer of BaseEntity
    BaseEntity* entity = entityDataModel().findEntityByName(searchName);
    entity->setDBAction(DBAction::dbaCREATE);
    mPickListSetting.selectedEntities.push_back(entity);
}

void PickListBrowser::onCloseSelection()
{
    done(0);
}


std::string PickListBrowser::typeID()
{
    return "PickListBrowser";
}
