#include <QDebug>

#include "valuelistdetaildlg.h"
#include "ui_valuelistdetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "baseentitydetaildlg.h"
#include "baseentity.h"
#include "valuelist.h"
#include "../utils/tools.h"

ValueListDetailDlg::ValueListDetailDlg(
                ValueList* entity, QDialog* parent) :
    BaseEntityDetailDlg{parent},
    ui{new Ui::ValueListDetailDlg},
    mEntity{entity}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();
}

ValueListDetailDlg::~ValueListDetailDlg()
{
    qDebug() << "ValueListDetailDlg::dtor";
    delete ui;
}

std::string ValueListDetailDlg::windowTitle()
{
    return mEntity->windowTitle();
}

ActionResult ValueListDetailDlg::saveRecord()
{
    populateEntityFields();
    ActionResult ar =  mEntity->validate();
    return ar;
}

void ValueListDetailDlg::populateFormWidgets()
{
    ui->edtCode->setText(stoq(mEntity->code()->value()));
    ui->edtValue->setText(stoq(mEntity->listValue()->value()));
}

void ValueListDetailDlg::populateEntityFields()
{
    mEntity->code()->setValue(ui->edtCode->text().toStdString());
    mEntity->listValue()->setValue(ui->edtValue->text().toStdString());
}
