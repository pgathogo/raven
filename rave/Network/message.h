#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

namespace NETWORK {

using SenderIPAddress = QString;
using SenderPort = int;
using MessageContent = QJsonObject;

enum class RequireResponse { Yes, No };

class Message {
public:
    explicit Message();
    explicit Message(SenderIPAddress, SenderPort, MessageContent,  RequireResponse require_response=RequireResponse::No);

    QString message_type();
    QJsonDocument message_data();
    void set_message_response(QJsonDocument);
    QJsonDocument message_response();

    friend QDataStream& operator<<(QDataStream& out, const Message& msg) ;
    friend QDataStream& operator>>(QDataStream& in,  Message& msg) ;

private:
    QJsonDocument m_message_data;
    SenderIPAddress m_sender_ipaddress;
    SenderPort m_sender_port;
    RequireResponse m_requires_response;
    QString m_message_type;
    QJsonDocument m_message_response;

};

}



#endif
