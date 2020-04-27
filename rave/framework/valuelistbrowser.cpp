#include "valuelistbrowser.h"
#include "ui_valuelistbrowser.h"

#include "valuelist.h"
#include "valuelistdetaildlg.h"

#include "entitydatamodel.h"
#include "baseentity.h"
#include "../utils/tools.h"

ValueListBrowser::ValueListBrowser(QWidget* parent,
                                   ValueList* entity):
        BaseEntityBrowserDlg(parent, entity),
        mEntity{entity},
        ui{new Ui::ValueListBrowser}
{
    ui->setupUi(this);
    setDialogTitle("Lookup");
}

ValueListBrowser::~ValueListBrowser()
{
    delete ui;
}

void ValueListBrowser::addRecord()
{
    addEntity<ValueList, ValueListDetailDlg>(mEntity);

    /*
    std::unique_ptr<ValueList> uvl = std::make_unique<ValueList>();
    auto ptr(uvl.get());
    ptr = mEntity;
    valueListDetailDlg = new ValueListDetailDlg(ptr);
    if (valueListDetailDlg->exec() > 0){
        std::unique_ptr<BaseEntity> be(ptr);
        entityDataModel()->createEntity(std::move(be));
    }
    */
}

void ValueListBrowser::updateRecord()
{
   update<ValueList, ValueListDetailDlg>();
}


