#include "valuelistbrowser.h"
#include "ui_valuelistbrowser.h"

#include "valuelist.h"
#include "valuelistdetaildlg.h"

#include "entitydatamodel.h"
#include "baseentity.h"
#include "../utils/tools.h"

ValueListBrowser::ValueListBrowser(const std::string& vltype,
                                   QWidget* parent)
    :BaseEntityBrowserDlg(parent,
                          ValueListBrowser::createValueList(vltype)),
     ui{new Ui::ValueListBrowser},
     vtype{vltype}
{
    qDebug() << "ValueList Ctor";
    ui->setupUi(this);
    setDialogTitle("Lookup");
}

ValueListBrowser::~ValueListBrowser()
{
    delete ui;
}

void ValueListBrowser::addRecord()
{
    auto ent = ValueListBrowser::createValueList(vtype);

    std::unique_ptr<ValueListDetailDlg> vdd =
            std::make_unique<ValueListDetailDlg>(ent.get());

    if (vdd->exec() > 0)
        entityDataModel().createEntity(std::move(ent));

}

void ValueListBrowser::updateRecord()
{
   update<ValueList, ValueListDetailDlg>();
}


std::string ValueListBrowser::typeID()
{
    return "ValueListBrowser";
}
