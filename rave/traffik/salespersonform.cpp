#include "salespersonform.h"
#include "ui_salespersonform.h"
#include "ui_baseentitydetaildlg.h"

#include "salesperson.h"

#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"

SalesPersonForm::SalesPersonForm(SalesPerson* sp,
                                 QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::SalesPersonForm),
    mSalesPerson{sp}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();

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
    populateEntityFields();
    ActionResult ar = mSalesPerson->validate();
    return ar;
}

void SalesPersonForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mSalesPerson->name()->value()));
    ui->edtMobile->setText(stoq(mSalesPerson->mobileno()->value()));
    ui->cbGender->setModel(mSalesPerson->gender()->dataModel());
    ui->cbGender->setCurrentIndex(ui->cbGender->findText(
                                 QString::fromStdString(mSalesPerson->gender()->displayName())));
}

void SalesPersonForm::populateEntityFields()
{
    mSalesPerson->name()->setValue(ui->edtName->text().toStdString());
    mSalesPerson->mobileno()->setValue(ui->edtMobile->text().toStdString());
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
