#ifndef NOTIFICATIONITEM_H
#define NOTIFICATIONITEM_H

#include <QWidget>
#include "types.h"

namespace Ui {
class NotificationItem;
}

class NotificationItem : public QWidget
{
    Q_OBJECT

public:
    explicit NotificationItem(QWidget *parent = nullptr);
    ~NotificationItem();

    std::string code();
    void setMessage(const std::string message,
                    NotificationType noticeType,
                    std::string styleSheet,
                    std::string fontColor);

private:
    Ui::NotificationItem *ui;
    std::string mCode;

};

#endif // NOTIFICATIONITEM_H
