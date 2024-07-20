#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

namespace NETWORK {

    using SenderIPAddress = QString;
    using SenderPort = int;
    using MessageContent = QJsonObject;

    enum class RequireResponse { Yes, No };

    class Message : public QObject {
        Q_OBJECT
    public:
        explicit Message();
        explicit Message(const SenderIPAddress, SenderPort, MessageContent,  RequireResponse require_response=RequireResponse::No);

        Message(const Message&);

        const QString& message_type() const { return m_message_type; }
        const QJsonDocument& message_data() const { return m_message_data; }
        const QJsonDocument& message_response() const { return m_message_response; }
        const QString& message_tostring() const { return m_message_data.toJson(QJsonDocument::Compact); }
        const QString& message_response_tostring() const { return m_message_response.toJson(QJsonDocument::Compact); }

        const QString& request() const { return m_request; }
        int need_response() const { return m_need_response; }

        void set_message_response(QJsonDocument);

        void test_emit(const QString);

        friend QDataStream& operator<<(QDataStream& out, Message& msg) ;
        friend QDataStream& operator>>(QDataStream& in,  Message& msg) ;

    signals:
        void log_message(const QString);

    private:
        QJsonDocument m_message_data;
        SenderIPAddress m_sender_ipaddress;
        SenderPort m_sender_port;
        RequireResponse m_requires_response;
        QString m_message_type;
        QJsonDocument m_message_response;

        QString m_request {""};
        int m_need_response {-1};

    };

}



#endif
