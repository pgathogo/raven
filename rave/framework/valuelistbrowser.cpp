#include "valuelistbrowser.h"
#include "ui_valuelistbrowser.h"

#include "valuelist.h"
#include "valuelistdetaildlg.h"

#include "entitydatamodel.h"
#include "baseentity.h"
#include "../utils/tools.h"

ValueListBrowser::ValueListBrowser(const std::string& vltype, QWidget* parent)
    :BaseEntityBrowserDlg(parent,
                          ValueListBrowser::createValueList(vltype)),
     ui{new Ui::ValueListBrowser},
     vdd{},
     vtype{vltype}
{
    qDebug() << "ValueList Ctor";
    ui->setupUi(this);
    setDialogTitle("Lookup");
}

ValueListBrowser::~ValueListBrowser()
{
    delete vdd;
    delete ui;
}

void ValueListBrowser::addRecord()
{
    ValueList* ent = ValueListBrowser::createValueList(vtype);
    vdd = new ValueListDetailDlg(ent);
    if (vdd->exec() > 0){
        ValueList vl;
        vl = *ent;
        entityDataModel()->createEntity(ent);
    }
}

void ValueListBrowser::updateRecord()
{
   update<ValueList, ValueListDetailDlg>();
}


std::string ValueListBrowser::typeID()
{
    return "ValueListBrowser";
}
