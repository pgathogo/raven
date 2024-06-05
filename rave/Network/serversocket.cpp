#include <QJsonObject>
#include <QTcpSocket>

#include "ServerSocket.h"
#include "requestresponsemanager.h"

namespace NETWORK
{

    ServerSocket::ServerSocket(const QString ipaddress, int port, QObject *parent)
        : m_ipaddress{ipaddress}
        , m_port{port}
        ,QTcpServer{parent}
        ,m_socket{nullptr}
    {
        connect(this, &ServerSocket::newConnection, this, &ServerSocket::on_new_connection);
        listen(QHostAddress(ipaddress), port);

        m_req_resp_manager = std::make_unique<NETWORK::RequestResponseManager>();

        emit log_server_message("Server is listening...");
    }

    void ServerSocket::incomingConnection(qintptr socket_descriptor)
    {
        if (m_socket) {
        QTcpSocket* new_socket = nextPendingConnection();
        new_socket->close();
        new_socket->deleteLater();

        } else {
        m_socket = new QTcpSocket(this);
        m_socket->setSocketDescriptor(socket_descriptor);
        connect(m_socket, &QTcpSocket::readyRead, this, &ServerSocket::read_socket);
        connect(m_socket, &QTcpSocket::disconnected, this, &ServerSocket::on_socket_disconnect);
        }
    }

    void ServerSocket::on_new_connection()
    { emit log_server_message("New connection established"); }

    void ServerSocket::read_socket()
    {
        emit log_server_message("> read_socket...");

        if (m_socket)
        {

            QDataStream socket_stream(m_socket);
            socket_stream.setVersion(QDataStream::Qt_6_3);

            socket_stream.startTransaction();

            Message message;
            socket_stream >> message;

            if (!socket_stream.commitTransaction()) {
                emit log_server_message("Waiting for more data...");
                return;
            }

            emit log_server_message(message.message_type());

            if (message.message_type() == "message")
            {
                message.set_message_response( m_req_resp_manager->process_message(message.message_data() ) );
                emit processed_message(message);
            }

        }
    }


    void ServerSocket::on_socket_disconnect()
    {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}
