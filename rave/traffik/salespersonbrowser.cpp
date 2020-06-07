#include "salespersonbrowser.h"
#include "ui_salespersonbrowser.h"

#include "salesperson.h"
#include "salespersonform.h"

SalesPersonBrowser::SalesPersonBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new SalesPerson),
    ui(new Ui::SalesPersonBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Sales Persons");
}

SalesPersonBrowser::~SalesPersonBrowser()
{
    delete ui;
}

void SalesPersonBrowser::addRecord()
{
    std::unique_ptr<SalesPerson> sp = std::make_unique<SalesPerson>();
    std::unique_ptr<SalesPersonForm> spForm = std::make_unique<SalesPersonForm>(sp.get());
    if (spForm->exec() > 0)
        entityDataModel()->createEntity(std::move(sp));
}

void SalesPersonBrowser::updateRecord()
{
   update<SalesPerson, SalesPersonForm>();
}

std::string SalesPersonBrowser::typeID()
{
    return "SalesPersonBrowser";
}
