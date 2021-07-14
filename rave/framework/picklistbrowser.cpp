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
    std::string search_name = selectedRowName().toStdString();

    if (search_name.empty())
        return;

    BaseEntity* entity = entityDataModel().findEntityByName(search_name);
    entity->setDBAction(DBAction::dbaCREATE); // ?????
    mPickListSetting.selectedEntities.push_back(entity);

    if (mPickListSetting.pickMode == PickListMode::plmSINGLE_SELECT)
        done(1);
}

void PickListBrowser::onCloseSelection()
{
    if (mPickListSetting.selectedEntities.size() > 0){
        done(1);
    }else{
        done(0);
    }
}


std::string PickListBrowser::typeID()
{
    return "PickListBrowser";
}
