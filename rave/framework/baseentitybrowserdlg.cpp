#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"

#include "baseentity.h"
#include "entitydatamodel.h"

BaseEntityBrowserDlg::BaseEntityBrowserDlg( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseEntityBrowserDlg),
    mBaseEntity{nullptr},
    mEntityDataModel{nullptr}
{
    ui->setupUi(this);
    connectSlots();
}

BaseEntityBrowserDlg::BaseEntityBrowserDlg(QWidget* parent,
                                           BaseEntity* entity)
        :QDialog(parent)
        ,ui(new Ui::BaseEntityBrowserDlg)
        ,mBaseEntity{entity}
        ,mEntityDataModel{new EntityDataModel(entity)}

{
    ui->setupUi(this);
    connectSlots();
    ui->tvEntity->setModel(mEntityDataModel);
    ui->tvEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

BaseEntityBrowserDlg::~BaseEntityBrowserDlg()
{
    delete ui;
}

void BaseEntityBrowserDlg::connectSlots()
{
    connect(ui->btnAdd, &QPushButton::clicked, this, &BaseEntityBrowserDlg::AddRecord);
    connect(ui->btnSearch, &QPushButton::clicked, this, &BaseEntityBrowserDlg::searchRecord);
}

void BaseEntityBrowserDlg::setDialogTitle(const QString title)
{
    this->setWindowTitle(title);
}

void BaseEntityBrowserDlg::searchRecord()
{
    qDebug() << "BaseEntityBrowserDlg()::searchRecord";
}

EntityDataModel* BaseEntityBrowserDlg::entityDataModel() const
{
    return mEntityDataModel;
}

void BaseEntityBrowserDlg::setMdiArea(QMdiArea* mdi)
{
    mMdiArea = mdi;
}
