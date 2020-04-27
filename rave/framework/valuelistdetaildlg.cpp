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

void ValueListDetailDlg::setEntity(BaseEntity* entity)
{
    //BaseEntity* valueList = dynamic_cast<BaseEntity*>(entity);

    qDebug() << "TableName: " << QString::fromStdString(mEntity->tableName());
}

ValueListDetailDlg::~ValueListDetailDlg()
{
    delete ui;
}

std::string ValueListDetailDlg::title()
{
    //return mEntity->title();
    return "title";
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
