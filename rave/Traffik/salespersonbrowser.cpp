#include "salespersonbrowser.h"
#include "ui_salespersonbrowser.h"

#include "salesperson.h"
#include "salespersonform.h"

SalesPersonBrowser::SalesPersonBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_shared<SalesPerson>()),
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
    auto sp = std::make_shared<SalesPerson>();

    std::unique_ptr<SalesPersonForm> spForm = std::make_unique<SalesPersonForm>(sp.get());

    if (spForm->exec() > 0)
        entityDataModel().createEntity(std::move(sp));
}

void SalesPersonBrowser::updateRecord()
{
   update<SalesPerson, SalesPersonForm>();
}

std::string SalesPersonBrowser::typeID()
{
    return "SalesPersonBrowser";
}
