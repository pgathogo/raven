#ifndef CLIENT_H
#define CLIENT_H

#include <memory>

#include <QObject>
#include <QTcpSocket>
#include "requestresponsemanager.h"

class QTcpSocket;
//class RequestResponseManager;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);
    void soc_connect();
    void soc_disconnect();
    void request(QString);

signals:
    void log_message(const QString);

private slots:
    void read_socket();
    void show_man_message(const QString);

private:
    void make_client_socket();
    void receive_request(QByteArray);

    std::unique_ptr<QTcpSocket> m_socket;
    std::unique_ptr<RequestResponseManager> m_req_resp_manager;
};

#endif // CLIENT_H
