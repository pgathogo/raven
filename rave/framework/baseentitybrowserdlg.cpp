#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"

#include "baseentity.h"
#include "entitydatamodel.h"

BaseEntityBrowserDlg::BaseEntityBrowserDlg( QWidget *parent) :
    QDialog(parent),
    bui(new Ui::BaseEntityBrowserDlg),
    mBaseEntity{nullptr},
    mEntityDataModel{nullptr}
{
    bui->setupUi(this);
    connectSlots();
}

BaseEntityBrowserDlg::BaseEntityBrowserDlg(QWidget* parent,
                                           BaseEntity* entity)
        :QDialog(parent)
        ,bui(new Ui::BaseEntityBrowserDlg)
        ,mBaseEntity{entity}
        ,mEntityDataModel{new EntityDataModel(entity)}

{
    bui->setupUi(this);
    connectSlots();
    bui->tvEntity->setModel(mEntityDataModel);
    bui->tvEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

BaseEntityBrowserDlg::~BaseEntityBrowserDlg()
{
    delete bui;
}

void BaseEntityBrowserDlg::connectSlots()
{
    connect(bui->btnAdd, &QPushButton::clicked, this, &BaseEntityBrowserDlg::addBtnClicked);
    connect(bui->btnEdit, &QPushButton::clicked, this, &BaseEntityBrowserDlg::editBtnClicked);
    connect(bui->btnSearch, &QPushButton::clicked, this, &BaseEntityBrowserDlg::searchBtnClicked);
}

void BaseEntityBrowserDlg::setDialogTitle(const QString title)
{
    this->setWindowTitle(title);
}

EntityDataModel* BaseEntityBrowserDlg::entityDataModel() const
{
    return mEntityDataModel;
}

void BaseEntityBrowserDlg::setMdiArea(QMdiArea* mdi)
{
    mMdiArea = mdi;
}

void BaseEntityBrowserDlg::addBtnClicked()
{
    addRecord();
}

void BaseEntityBrowserDlg::searchBtnClicked()
{
    searchRecord();
}

void BaseEntityBrowserDlg::editBtnClicked()
{
    updateRecord();
}

int BaseEntityBrowserDlg::selectedRowId()
{
    return bui->tvEntity->currentIndex().row();
}

QString BaseEntityBrowserDlg::selectedRowName()
{

    QVariant col_name = bui->tvEntity->model()->data(
                            bui->tvEntity->model()->index(
                                    selectedRowId(), 0));
    return col_name.toString();
}
