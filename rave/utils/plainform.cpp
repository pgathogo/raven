#include <QPushButton>
#include <QDebug>
#include "plainform.h"
#include "../framework/baseentitydetaildlg.h"
#include "ui_plainform.h"
#include "../utils/daypartgrid.h"

PlainForm::PlainForm(QDialog *parent) :
    ui(new Ui::PlainForm)
{
    ui->setupUi(this);
    connect(ui->btnShow, &QPushButton::clicked,this, &PlainForm::onShow);
    dpg = new DayPartGrid(ui->vlGrid);
    fillGrid();
}

PlainForm::~PlainForm()
{
    delete dpg;
    delete ui;
}

void PlainForm::onShow()
{
    auto dp = dpg->readGrid();
    for (const auto& [key, value] : dp)
        qDebug() << QString::fromStdString(key)+" : "<< QString::fromStdString(value) ;
}

void PlainForm::fillGrid()
{
    std::map<std::string, std::string> dayparts;
    std::string dp1 {"000000001100011000000000"};
    std::string dp2 {"000000001100011000000000"};
    std::string dp3 {"000000001100011000000000"};
    std::string dp4 {"000000001100011000000000"};
    std::string dp5 {"000000001100011000000000"};
    std::string dp6 {"000000001100011000000000"};
    std::string dp7 {"000000001100011000000000"};
    dayparts["daypart1"]=dp1;
    dayparts["daypart2"]=dp2;
    dayparts["daypart3"]=dp3;
    dayparts["daypart4"]=dp4;
    dayparts["daypart5"]=dp5;
    dayparts["daypart6"]=dp6;
    dayparts["daypart7"]=dp7;
    dpg->updateGrid(dayparts);
}
