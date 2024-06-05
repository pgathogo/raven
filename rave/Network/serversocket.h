#ifndef CODEXSERVER_H
#define CODEXSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QJsonDocument>

#include "message.h"

class QTcpSocket;


namespace NETWORK
{

class RequestResponseManager;

   /*
    struct Message {
        QString message_type;
        QString sender_ipaddress{""};
        int sender_port{-1};
        QByteArray message;
        QJsonDocument response;
        bool response_required{false};
    };
   */

    class ServerSocket : public QTcpServer
    {
        Q_OBJECT
    public:
        explicit ServerSocket(const QString, int, QObject *parent = nullptr);

    protected:
        void incomingConnection(qintptr socket_descriptor) override;

    signals:
        void log_server_message(const QString);
        void processed_message(Message);

    private slots:
        void on_new_connection();
        void read_socket();
        void on_socket_disconnect();

    private:
        QTcpSocket* m_socket;
        std::unique_ptr<NETWORK::RequestResponseManager> m_req_resp_manager;
        QString m_ipaddress;
        int m_port;
    };

}

#endif // CODEXSERVER_H
