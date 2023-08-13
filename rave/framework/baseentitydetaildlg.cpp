#include <QListWidgetItem>
#include "baseentitydetaildlg.h"
#include "ui_baseentitydetaildlg.h"
#include "entitydatamodel.h"
#include "../utils/notificationbar.h"
#include "../utils/tools.h"

BaseEntityDetailDlg::BaseEntityDetailDlg(QDialog *parent) :
    QDialog(parent),
    bui(new Ui::BaseEntityDetailDlg),
    mNoticeBar{},
    m_okay_to_close{true}
{
    bui->setupUi(this);
    connectSlots();
    mNoticeBar = new NotificationBar(bui->noticeLayout);
    hideSaveNewBtn();
}


std::string BaseEntityDetailDlg::title()
{
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
//    connect(bui->btnSaveNew, &QPushButton::clicked, this, &BaseEntityDetailDlg::btnSaveNewClicked);
    connect(this, SIGNAL(dialog_is_closing()), this, SLOT(onCloseDialog()));
}

void BaseEntityDetailDlg::closeEvent(QCloseEvent* event)
{
    if(!m_okay_to_close){
        if (confirmationMessage("Exit without saving?"))
            done(0);
        else
            event->ignore();
    }
}

const BrowserForms &BaseEntityDetailDlg::get_forms() const
{
    return mForms;
}

void BaseEntityDetailDlg::btnSaveClicked()
{
    ActionResult ar = saveRecord();

    if (std::get<0>(ar) == ActionResultType::arSUCCESS){
       m_okay_to_close = true;
       done(1);
    }else{
       mNoticeBar->errorNotification(std::get<1>(ar));
       //m_okay_to_close = false;
    }
}

void BaseEntityDetailDlg::btnCloseClicked()
{
    if(!m_okay_to_close){
        if (confirmationMessage("Exit without saving?"))
            done(0);
    }else{
        done(0);
    }
}


void BaseEntityDetailDlg::btnSaveNewClicked()
{
    ActionResult ar = saveRecord();

    if (std::get<0>(ar) == ActionResultType::arSUCCESS){
        mNoticeBar->successNotification("Success");
        clear_widgets();
        // clear fields
    } else {
        mNoticeBar->errorNotification(std::get<1>(ar));
        m_okay_to_close = false;
    }
}

void BaseEntityDetailDlg::clear_widgets()
{

}


void BaseEntityDetailDlg::onCloseDialog()
{

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

void BaseEntityDetailDlg::disableSaveBtn()
{
    bui->btnSave->setEnabled(false);
}

void BaseEntityDetailDlg::hideSaveBtn()
{
    bui->btnSave->setVisible(false);
}

void BaseEntityDetailDlg::disableSaveNewBtn()
{
    bui->btnSaveNew->setEnabled(false);
}

void BaseEntityDetailDlg::hideSaveNewBtn()
{
    bui->btnSaveNew->setVisible(false);
}

