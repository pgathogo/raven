#include "salespersonbrowser.h"
#include "ui_salespersonbrowser.h"

#include "salesperson.h"
#include "salespersonform.h"

SalesPersonBrowser::SalesPersonBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new SalesPerson),
    ui(new Ui::SalesPersonBrowser),
    mSalesPerson{}
{
    ui->setupUi(this);
    setDialogTitle("Sales Persons");
}

SalesPersonBrowser::~SalesPersonBrowser()
{
    delete mSalesPerson;
    delete ui;
}

void SalesPersonBrowser::addRecord()
{
    mSalesPerson = new SalesPerson();
    mSalesPersonForm = new SalesPersonForm(mSalesPerson);
    if (mSalesPersonForm->exec() > 0)
        entityDataModel()->createEntity(mSalesPerson);
}

void SalesPersonBrowser::updateRecord()
{
   update<SalesPerson, SalesPersonForm>();
}

std::string SalesPersonBrowser::typeID()
{
    return "SalesPersonBrowser";
}
