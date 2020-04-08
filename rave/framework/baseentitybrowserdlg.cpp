#include <QMessageBox>

#include "baseentitybrowserdlg.h"
#include "ui_baseentitybrowserdlg.h"

#include "baseentity.h"
#include "entitydatamodel.h"

#include "../utils/tools.h"

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
    connect(bui->btnDelete, &QPushButton::clicked, this, &BaseEntityBrowserDlg::deleteBtnClicked);
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

void BaseEntityBrowserDlg::editBtnClicked()
{
    if (selectedRowId() < 0){
        showMessage("Select a record to edit.");
    }else{
        updateRecord();
    }
}

void BaseEntityBrowserDlg::deleteBtnClicked()
{
    if (selectedRowId() < 0){
        showMessage("Select a record to delete.");
    }else{
        if (QMessageBox::question(this, tr("Traffik"),
                                    tr("Are you sure you want to delete the record?"),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)

        deleteRecord();
    }
}

void BaseEntityBrowserDlg::searchBtnClicked()
{
    searchRecord();
}

int BaseEntityBrowserDlg::selectedRowId() const
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

void BaseEntityBrowserDlg::removeSelectedRow()
{
    bui->tvEntity->model()->removeRow(selectedRowId());
}

void BaseEntityBrowserDlg::updateTableViewRecord(BaseEntity* entity)
{
    std::vector<std::string> values = entity->tableViewValues();
    int col = 0;
    for (auto value: values){
        QVariant qvalue(QString::fromStdString(value));
        QModelIndex modelIndex = bui->tvEntity->model()->index(selectedRowId(), col);
        QAbstractItemModel* model = bui->tvEntity->model();
        model->setData(modelIndex, qvalue);
        ++col;
    }
}
