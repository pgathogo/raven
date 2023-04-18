#include <QDebug>
#include "picklistbrowser.h"
#include "ui_picklistbrowser.h"

#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"
#include "selectclosewidget.h"
#include "manytomany.h"

PickListSetting::PickListSetting(std::shared_ptr<BaseEntity> entity,
                                 int search_col_index,
                                 PickListMode plm)
    :listEntity{std::move(entity)}
    ,pickMode{plm}
    ,m_search_col_index{search_col_index}
{
}

PickListBrowser::PickListBrowser(PickListSetting& plSetting,
                                 QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::move(plSetting.listEntity)),
    ui(new Ui::PickListBrowser),
    scw{},
    m_picklist_setting{plSetting}
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

  /*
    std::string search_col_name = entityDataModel().getEntity().searchColumn();
    int search_col_index = get_column_index(search_col_name);:23

    if (search_col_index < 0 )
        return;
    */

    std::string search_name = selectedRowName(m_picklist_setting.m_search_col_index).toStdString();

    if (search_name.empty())
        return;

    std::shared_ptr<BaseEntity> entity = entityDataModel().findEntityByName(search_name);
    entity->setDBAction(DBAction::dbaCREATE); // ?????
    m_picklist_setting.selectedEntities.push_back(entity.get());

    if (m_picklist_setting.pickMode == PickListMode::plmSINGLE_SELECT)
        done(1);
}

void PickListBrowser::onCloseSelection()
{
    if (m_picklist_setting.selectedEntities.size() > 0){
        done(1);
    }else{
        done(0);
    }
}

int PickListBrowser::get_column_index(std::string column_name)
{
    for( int i=0; i<bui->tvEntity->model()->columnCount(); ++i){
        auto col_name = bui->tvEntity->model()->headerData(i, Qt::Horizontal).toString();
        if (col_name == QString::fromStdString(column_name)){
            return i;
        }
    }

    return -1;
}


std::string PickListBrowser::typeID()
{
    return "PickListBrowser";
}
