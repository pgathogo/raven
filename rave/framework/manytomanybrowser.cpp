#include <QVBoxLayout>

#include "manytomanybrowser.h"
#include "ui_manytomanybrowser.h"

#include "picklistbrowser.h"
#include "manytomany.h"

ManyToManyBrowser::ManyToManyBrowser(
        const std::string typeInfo,
        BaseEntity* pEnt,
        BaseEntity* dEnt,
        QVBoxLayout* layout,
        QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         ManyToManyBrowser::createMToM(typeInfo, pEnt, dEnt)),
    ui(new Ui::ManyToManyBrowser),
    plb{},
    mParentEntity{pEnt},
    mDetailEntity{dEnt}
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
    plb = new PickListBrowser(mDetailEntity, entityDataModel());
    plb->exec();
}

void ManyToManyBrowser::updateRecord()
{
}

void ManyToManyBrowser::selectedItem(BaseEntity* entity)
{

    qDebug() << "ManyToManyBrowser::selectedItem";
    qDebug() << QString::fromStdString(entity->tableName());
}
