#ifndef NOTIFICATIONBAR_H
#define NOTIFICATIONBAR_H

#include <QObject>

#include "types.h"

class QVBoxLayout;
class QTimer;
class NotificationItem;


class NotificationBar : public QObject
{
    Q_OBJECT
public:
    explicit NotificationBar(QVBoxLayout* layout, std::size_t timeInterval=5000, QObject* parent = nullptr);
    ~NotificationBar();
    void insertNotification(std::string message, NotificationType nType);
    void errorNotification(std::string message);
    void successNotification(std::string message);

signals:

public slots:
    void clear();

private:
    QVBoxLayout* mLayout;
    QTimer* mTimer;
    std::map<std::string, NotificationItem*> mNotifications;
    std::size_t mTimeInterval;
    std::string errorStyleSheet;
    std::string errorFontColor;
    std::string successStyleSheet;
    std::string successFontColor;
};

#endif // NOTIFICATIONBAR_H
