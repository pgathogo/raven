#include <QPixmap>

#include "notificationitem.h"
#include "ui_notificationitem.h"

#include "../utils/tools.h"

NotificationItem::NotificationItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotificationItem),
    mCode{uuid::generate_uuid_v4()}
{
    ui->setupUi(this);
    QPixmap pixmap(":/images/icons/images/icons/close_msg.png");
    ui->lblClose->setPixmap(pixmap);
}

NotificationItem::~NotificationItem()
{
    delete ui;
}

std::string NotificationItem::code()
{
    return mCode;
}

void NotificationItem::setMessage(const std::string message,
                    NotificationType noticeType,
                    std::string styleSheet,
                    std::string fontColor)
{
    QPixmap noticePixmap;
    if (noticeType == NotificationType::ntERROR)
        noticePixmap = QPixmap(":/images/icons/images/icons/remove.png");

    ui->lblMessage->setText(QString::fromStdString(message));
    ui->lblIcon->setPixmap(noticePixmap);
    ui->frame->setStyleSheet(QString::fromStdString(styleSheet));
    ui->lblMessage->setStyleSheet(QString::fromStdString(fontColor));

}
