#include "salespersonform.h"
#include "ui_salespersonform.h"
#include "ui_baseentitydetaildlg.h"

#include "salesperson.h"

#include "../framework/entitydatamodel.h"

SalesPersonForm::SalesPersonForm(SalesPerson* sp,
                                 QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::SalesPersonForm),
    mSalesPerson{sp}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(windowTitle());

    connect(ui->cbGender, SIGNAL(currentIndexChanged(int)),
            this, SLOT(genderComboChanged(int)));
}

SalesPersonForm::~SalesPersonForm()
{
    //delete mSalesPerson;
    delete ui;
}


ActionResult SalesPersonForm::saveRecord()
{
    mSalesPerson->populateEntity();
    ActionResult ar = mSalesPerson->validate();
    return ar;
}

void SalesPersonForm::bindWidgets()
{
    mSalesPerson->name()->setWidget(ui->edtName);
    mSalesPerson->mobileno()->setWidget(ui->edtMobile);
    mSalesPerson->gender()->setWidget(ui->cbGender);
}

std::string SalesPersonForm::windowTitle()
{
    return "Sales Person Details";
}

void SalesPersonForm::genderComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbGender->model());
    mSalesPerson->gender()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
