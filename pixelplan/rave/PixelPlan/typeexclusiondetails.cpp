#include <QDebug>
#include "typeexclusiondetails.h"
#include "ui_typeexclusiondetails.h"
#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/utils/daypartgrid.h"
#include "../../../rave/utils/tools.h"

//#include "../../../rave/audio/trackpickerdialog.h"

TypeExclusionDetails::TypeExclusionDetails(
    TRAFFIK::TypeExclusion* tex, QDialog *parent)
    : BaseEntityDetailDlg(parent)
    , ui(new Ui::TypeExclusionDetails)
    , m_typeEx{tex}
    , dpg{}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    ui->tabWidget->setCurrentIndex(0);
    dpg = new DayPartGrid(ui->vlTypeEx);

    populateFormWidgets();
    populateGrid();

    //auto track_dialog = new AUDIO::TrackPickerDialog();
}

TypeExclusionDetails::~TypeExclusionDetails()
{
    delete ui;
    delete dpg;
}


std::string TypeExclusionDetails::windowTitle()
{
    return "Type Exclusions";
}

ActionResult TypeExclusionDetails::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_typeEx->validate();
    return ar;
}

void TypeExclusionDetails::populateFormWidgets()
{
    ui->edtName->setText(stoq(m_typeEx->name()->value()));
    ui->edtDesc->setText(stoq(m_typeEx->description()->value()));
}

void TypeExclusionDetails::populateEntityFields()
{
    m_typeEx->name()->setValue(ui->edtName->text().toStdString());
    m_typeEx->description()->setValue(ui->edtDesc->toPlainText().toStdString());

    auto dayparts = dpg->read_grid();
    m_typeEx->setDaypart1(dayparts[1]);
    m_typeEx->setDaypart2(dayparts[2]);
    m_typeEx->setDaypart3(dayparts[3]);
    m_typeEx->setDaypart4(dayparts[4]);
    m_typeEx->setDaypart5(dayparts[5]);
    m_typeEx->setDaypart6(dayparts[6]);
    m_typeEx->setDaypart7(dayparts[7]);

}

void TypeExclusionDetails::populateGrid()
{
    std::map<int, std::string> dayparts;
    dayparts[1] = m_typeEx->daypart1()->valueToString();
    dayparts[2] = m_typeEx->daypart2()->valueToString();
    dayparts[3] = m_typeEx->daypart3()->valueToString();
    dayparts[4] = m_typeEx->daypart4()->valueToString();
    dayparts[5] = m_typeEx->daypart5()->valueToString();
    dayparts[6] = m_typeEx->daypart6()->valueToString();
    dayparts[7] = m_typeEx->daypart7()->valueToString();
    dpg->update_grid(dayparts);
}
