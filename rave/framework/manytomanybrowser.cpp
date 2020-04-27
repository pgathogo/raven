#include <QVBoxLayout>

#include "manytomanybrowser.h"
#include "ui_manytomanybrowser.h"

#include "picklistbrowser.h"

ManyToManyBrowser::ManyToManyBrowser(QVBoxLayout* layout,
                                     QWidget *parent) :
    BaseEntityBrowserDlg(parent),
    ui(new Ui::ManyToManyBrowser),
    plb{}
{
    ui->setupUi(this);
    hideEditButton();
    layout->addWidget(this);
}

ManyToManyBrowser::~ManyToManyBrowser()
{
    delete plb;
    delete ui;
}

void ManyToManyBrowser::addRecord()
{
    plb = new PickListBrowser();
    plb->exec();
}

void ManyToManyBrowser::updateRecord()
{
}
