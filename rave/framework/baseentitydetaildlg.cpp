
#include "baseentitydetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "entitydatamodel.h"
#include "../utils/notificationbar.h"
#include "../utils/tools.h"

BaseEntityDetailDlg::BaseEntityDetailDlg(QDialog *parent) :
    QDialog(parent),
    bui(new Ui::BaseEntityDetailDlg),
    mNoticeBar{},
    mOkClose{true}
{
    bui->setupUi(this);
    connectSlots();
    //setWindowTitle(QString::fromStdString(title()));
    mNoticeBar = new NotificationBar(bui->noticeLayout);
}


std::string BaseEntityDetailDlg::title(){
    return "Details Dialog";
}


BaseEntityDetailDlg::~BaseEntityDetailDlg()
{
    qDebug() << "BaseEntityDetailDlg::dtor" ;
    delete mNoticeBar;
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
    done(0);
    if(!mOkClose)
        event->ignore();
}

void BaseEntityDetailDlg::btnSaveClicked()
{
    ActionResult ar = saveRecord();

    if (std::get<0>(ar) == ActionResultType::arSUCCESS){
       mOkClose = true;
       done(1);
    }else{
       mNoticeBar->errorNotification(std::get<1>(ar));
       mOkClose = false;
    }
}

void BaseEntityDetailDlg::btnCloseClicked()
{
    done(0);
}

void BaseEntityDetailDlg::btnSaveNewClicked()
{
   //mNoticeBar->errorNotification("Testing");
   mNoticeBar->successNotification("Success");
}
void BaseEntityDetailDlg::save(BaseEntity* entity)
{
}

void BaseEntityDetailDlg::setTitle(std::string title)
{
    QString qtitle = QString::fromStdString(title);
    setWindowTitle(qtitle);
    bui->lblTitle->setText(qtitle);
}
