#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

#include "notificationbar.h"
#include "notificationitem.h"

NotificationBar::NotificationBar(QVBoxLayout* layout, std::size_t timeInterval, QObject* parent)
        :mLayout{nullptr}
        ,mTimer{nullptr}
        ,mNotifications{{"",nullptr}}
        ,mTimeInterval{timeInterval}
{
    errorStyleSheet = "background-color: #FFBABA;";
    errorFontColor = "color: #D8000C;";

    successStyleSheet = "background-color: #DFF2BF;";
    successFontColor = "color: #4F8A10;";

    mLayout = new QVBoxLayout();
    mLayout = layout;

    mTimer = new QTimer();
    mTimer->setInterval(static_cast<int>(mTimeInterval));

    connect(this->mTimer, &QTimer::timeout, this, &NotificationBar::clear);

}

NotificationBar::~NotificationBar()
{
    delete mLayout;
    delete mTimer;
}

void NotificationBar::insertNotification(std::string message, NotificationType nType)
{
    std::string frameStyle;
    std::string fontColor;

    if (mLayout != nullptr){
        NotificationItem* nItem = new NotificationItem();

        if (nType == NotificationType::ntERROR){
            frameStyle = errorStyleSheet;
            fontColor = errorFontColor;
        } else if (nType == NotificationType::ntSUCCESS){
            frameStyle = successStyleSheet;
            fontColor = successFontColor;
        }

        nItem->setMessage(message, nType, frameStyle, fontColor);
        mLayout->addWidget(nItem);
        mNotifications[nItem->code()] = nItem;
        mTimer->start();
    }
}

void NotificationBar::errorNotification(std::string message)
{
    this->insertNotification(message, NotificationType::ntERROR);
}

void NotificationBar::successNotification(std::string message)
{
    this->insertNotification(message, NotificationType::ntSUCCESS);
}

void NotificationBar::clear()
{
    qDebug() << "Clear notification";
    if (mLayout != nullptr){
        for (auto item : mNotifications){
            mLayout->removeWidget(item.second);
            delete item.second;
            //item.second->deleteLater();
        }
    mTimer->stop();
    mNotifications.erase(mNotifications.begin(), mNotifications.end());
    }
}
