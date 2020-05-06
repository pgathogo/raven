#include <QDebug>

#include "valuelistdetaildlg.h"
#include "ui_valuelistdetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "baseentitydetaildlg.h"
#include "baseentity.h"
#include "valuelist.h"

ValueListDetailDlg::ValueListDetailDlg(
                ValueList* entity, QDialog* parent) :
    BaseEntityDetailDlg{parent},
    ui{new Ui::ValueListDetailDlg},
    mEntity{entity}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(title());

}

ValueListDetailDlg::~ValueListDetailDlg()
{
    qDebug() << "ValueListDetailDlg::dtor";
    delete ui;
}

std::string ValueListDetailDlg::title()
{
    return mEntity->windowTitle();
}

void ValueListDetailDlg::bindWidgets()
{
    mEntity->code()->setWidget(ui->edtCode);
    mEntity->listValue()->setWidget(ui->edtValue);
}

ActionResult ValueListDetailDlg::saveRecord()
{
    mEntity->populateEntity();
    ActionResult ar =  mEntity->validate();
    return ar;
}
