#include "baseentitydetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "entitydatamodel.h"

BaseEntityDetailDlg::BaseEntityDetailDlg(QDialog *parent) :
    QDialog(parent),
    bui(new Ui::BaseEntityDetailDlg)
{
    bui->setupUi(this);
    connectSlots();
    //setModal(true);
}

BaseEntityDetailDlg::~BaseEntityDetailDlg()
{
    delete bui;
}

void BaseEntityDetailDlg::connectSlots()
{
    connect(bui->btnSave, &QPushButton::clicked, this, &BaseEntityDetailDlg::btnSaveClicked);
    connect(bui->btnClose, &QPushButton::clicked, this, &BaseEntityDetailDlg::btnCloseClicked);
}

void BaseEntityDetailDlg::btnSaveClicked()
{
    saveRecord();
    done(1);
}

void BaseEntityDetailDlg::btnCloseClicked()
{
    done(0);
}

void BaseEntityDetailDlg::save(BaseEntity* entity)
{
    //mEntityDataModel->saveEntity(entity);
}

void BaseEntityDetailDlg::setTitle(std::string title)
{
    bui->lblTitle->setText(QString::fromStdString(title));
}
