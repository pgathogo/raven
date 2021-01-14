#include <QDebug>
#include "typeexclusiondetails.h"
#include "ui_typeexclusiondetails.h"
#include "ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../utils/tools.h"

TypeExclusionDetails::TypeExclusionDetails(
        TRAFFIK::TypeExclusion* tex, QDialog *parent) :
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

    auto dayparts = dpg->read_grid();
    typeEx->setDaypart1(dayparts[1]);
    typeEx->setDaypart2(dayparts[2]);
    typeEx->setDaypart3(dayparts[3]);
    typeEx->setDaypart4(dayparts[4]);
    typeEx->setDaypart5(dayparts[5]);
    typeEx->setDaypart6(dayparts[6]);
    typeEx->setDaypart7(dayparts[7]);

}

void TypeExclusionDetails::populateGrid()
{
    std::map<int, std::string> dayparts;
    dayparts[1] = typeEx->daypart1()->valueToString();
    dayparts[2] = typeEx->daypart2()->valueToString();
    dayparts[3] = typeEx->daypart3()->valueToString();
    dayparts[4] = typeEx->daypart4()->valueToString();
    dayparts[5] = typeEx->daypart5()->valueToString();
    dayparts[6] = typeEx->daypart6()->valueToString();
    dayparts[7] = typeEx->daypart7()->valueToString();
    dpg->update_grid(dayparts);
}
