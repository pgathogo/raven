#include <QDebug>
#include "typeexclusiondetails.h"
#include "ui_typeexclusiondetails.h"
#include "ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "typeexclusion.h"
#include "../utils/daypartgrid.h"
#include "../utils/tools.h"

TypeExclusionDetails::TypeExclusionDetails(
        TypeExclusion* tex, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::TypeExclusionDetails),
    typeEx{tex},
    dpg{}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    ui->tabWidget->setCurrentIndex(0);
    dpg = new DayPartGrid(ui->vlTypeEx);

    populateFormWidgets();
    populateGrid();
}

TypeExclusionDetails::~TypeExclusionDetails()
{
    delete ui;
    //delete typeEx;
    delete dpg;
    qDebug() << "TypeExclusion dtor";
}


std::string TypeExclusionDetails::windowTitle()
{
    return "Type Exclusions";
}

ActionResult TypeExclusionDetails::saveRecord()
{
    populateEntityFields();
    ActionResult ar = typeEx->validate();
    return ar;
}

void TypeExclusionDetails::populateFormWidgets()
{
    ui->edtName->setText(stoq(typeEx->name()->value()));
    ui->edtDesc->setText(stoq(typeEx->description()->value()));
}

void TypeExclusionDetails::populateEntityFields()
{
    typeEx->name()->setValue(ui->edtName->text().toStdString());
    typeEx->description()->setValue(ui->edtDesc->toPlainText().toStdString());

    auto dayparts = dpg->readGrid();
    typeEx->setDaypart1(dayparts["daypart1"]);
    typeEx->setDaypart2(dayparts["daypart2"]);
    typeEx->setDaypart3(dayparts["daypart3"]);
    typeEx->setDaypart4(dayparts["daypart4"]);
    typeEx->setDaypart5(dayparts["daypart5"]);
    typeEx->setDaypart6(dayparts["daypart6"]);
    typeEx->setDaypart7(dayparts["daypart7"]);

}

void TypeExclusionDetails::populateGrid()
{
    std::map<std::string, std::string> dayparts;
    dayparts["daypart1"] = typeEx->daypart1()->valueToString();
    dayparts["daypart2"] = typeEx->daypart2()->valueToString();
    dayparts["daypart3"] = typeEx->daypart3()->valueToString();
    dayparts["daypart4"] = typeEx->daypart4()->valueToString();
    dayparts["daypart5"] = typeEx->daypart5()->valueToString();
    dayparts["daypart6"] = typeEx->daypart6()->valueToString();
    dayparts["daypart7"] = typeEx->daypart7()->valueToString();
    dpg->updateGrid(dayparts);
}
