#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>

#include "message.h"

class QJsonDocument;

using DestIPAddress = QString;
using DestPort = int;

namespace NETWORK
{

class RequestResponseManager;

    class ClientSocket : public QObject
    {
        Q_OBJECT
    public:
        explicit ClientSocket(QObject *parent = nullptr);
        void soc_connect();
        void soc_disconnect();
        void send_message(Message, DestIPAddress, DestPort );
        //void send_message(QJsonDocument, SenderIpAddress, SenderPort);

    signals:
        void log_message(const QString);

    // private slots:
    //     void read_socket();

    private:
        std::unique_ptr<QTcpSocket> m_socket;
        std::unique_ptr<RequestResponseManager> m_req_resp_manager;

        void make_client_socket();
        void receive_request(QByteArray);
    };

 }

#endif // CLIENTSOCKET_H
