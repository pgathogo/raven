#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>

#include "../../rave/codex/requestresponsemanager.h"



class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent = nullptr);
    void soc_connect();
    void soc_disconnect();
    void send_request(const QString);

signals:
    void log_message(const QString);

private slots:
    void read_socket();

private:
    void make_client_socket();
    void receive_request(QByteArray);

    std::unique_ptr<QTcpSocket> m_socket;
    std::unique_ptr<RequestResponseManager> m_req_resp_manager;

};

#endif // CLIENTSOCKET_H
