#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QTcpServer>

class QTcpSocket;
class RequestResponseManager;


struct RequestSender {
    QString ipaddress{""};
    int port{-1};
};

class ServerSocket : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServerSocket(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socket_descriptor) override;

signals:
    void log_server_message(const QString);

protected slots:
    void on_new_connection();
    void read_socket();
    void on_socket_disconnect();

private:
    std::unique_ptr<QTcpSocket> m_socket;
    std::unique_ptr<RequestResponseManager> m_req_resp_manager;

    void receive_request(RequestSender, QByteArray);
};

#endif // SERVERSOCKET_H
