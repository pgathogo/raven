#include "baseentitydetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "entitydatamodel.h"

BaseEntityDetailDlg::BaseEntityDetailDlg(EntityDataModel* edm, QWidget *parent) :
    QWidget(parent),
    bui(new Ui::BaseEntityDetailDlg),
    mEntityDataModel{edm}
{
    bui->setupUi(this);
    connectSlots();
}

BaseEntityDetailDlg::~BaseEntityDetailDlg()
{
    delete bui;
}

void BaseEntityDetailDlg::connectSlots()
{
    connect(bui->btnSave, &QPushButton::clicked, this, &BaseEntityDetailDlg::saveRecord);
}


EntityDataModel* BaseEntityDetailDlg::entityDataModel()
{
    return mEntityDataModel;
}

void BaseEntityDetailDlg::save(BaseEntity* entity)
{
    mEntityDataModel->saveEntity(entity);
}
