
#include "baseentitydetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "entitydatamodel.h"
#include "../utils/notificationbar.h"
#include "../utils/tools.h"

BaseEntityDetailDlg::BaseEntityDetailDlg(QDialog *parent) :
    QDialog(parent),
    bui(new Ui::BaseEntityDetailDlg),
    mNoticeBar{nullptr},
    mOkClose{true}
{
    bui->setupUi(this);
    connectSlots();
    mNoticeBar = new NotificationBar(bui->noticeLayout);
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
    connect(bui->btnSaveNew, &QPushButton::clicked, this, &BaseEntityDetailDlg::btnSaveNewClicked);
}

void BaseEntityDetailDlg::closeEvent(QCloseEvent* event)
{
    qDebug() << "mOkClose: "<< mOkClose;
    if(!mOkClose)
        event->ignore();
}

void BaseEntityDetailDlg::btnSaveClicked()
{
    ErrorMessage em = saveRecord();

    if (std::get<0>(em)){
       mOkClose = true;
       done(1);
    }else{
       mOkClose = false;
    }
}

void BaseEntityDetailDlg::btnCloseClicked()
{
    done(0);
}

void BaseEntityDetailDlg::btnSaveNewClicked()
{
   mNoticeBar->errorNotification("testing");
}
void BaseEntityDetailDlg::save(BaseEntity* entity)
{
}

void BaseEntityDetailDlg::setTitle(std::string title)
{
    bui->lblTitle->setText(QString::fromStdString(title));
}
