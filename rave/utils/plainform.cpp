#include <QPushButton>
#include <QDebug>
#include "plainform.h"
#include "../framework/baseentitydetaildlg.h"
#include "ui_plainform.h"
#include "../utils/daypartgrid.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/entitydatamodel.h"

PlainForm::PlainForm(QDialog *parent) :
    ui(new Ui::PlainForm),
    dpg{},
    mtom{}
{
    ui->setupUi(this);
    connect(ui->btnShow, &QPushButton::clicked,this, &PlainForm::onShow);
    connect(ui->btnClose, &QPushButton::clicked,this, &PlainForm::onClose);
    //dpg = new DayPartGrid(ui->vlGrid);
    //fillGrid();
   // mtom = new ManyToManyBrowser(ui->vlGrid);
}

PlainForm::~PlainForm()
{
    delete dpg;
    delete mtom;
    delete edm;
    delete ui;
}

void PlainForm::onShow()
{
    edm = new EntityDataModel();
    //auto dp = dpg->readGrid();
    //for (const auto& [key, value] : dp)
    //    qDebug() << QString::fromStdString(key)+" : "<< QString::fromStdString(value) ;
}

void PlainForm::onClose()
{
    done(1);
}

void PlainForm::fillGrid()
{
    std::map<int, std::string> dayparts;

    dayparts[1]= "000000001100011000000000";
    dayparts[2]= "000000001100011000000000";
    dayparts[3]= "000000001100011000000000";
    dayparts[4]= "000000001100011000000000";
    dayparts[5]= "000000001100011000000000";
    dayparts[6]= "000000001100011000000000";
    dayparts[7]= "000000001100011000000000";

    dpg->update_grid(dayparts);
}
