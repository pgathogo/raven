#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

#include "serversocket.h"
#include "../../../rave/codex/requestresponsemanager.h"

ServerSocket::ServerSocket(QObject *parent)
    : QTcpServer{parent}
    , m_socket{nullptr}
{
    connect(this, &ServerSocket::newConnection, this, &ServerSocket::on_new_connection);
    listen(QHostAddress("127.0.0.1"), 5544);

    m_req_resp_manager = std::make_unique<RequestResponseManager>();

    emit log_server_message("Clusterman: Server is listening...");
}

void ServerSocket::incomingConnection(qintptr socket_descriptor)
{
    if (m_socket) {
        QTcpSocket* new_socket = nextPendingConnection();
        new_socket->close();
        new_socket->deleteLater();

    } else {
        m_socket = std::make_unique<QTcpSocket>(this);
        m_socket->setSocketDescriptor(socket_descriptor);
        connect(m_socket.get(), &QTcpSocket::readyRead, this, &ServerSocket::read_socket);
        connect(m_socket.get(), &QTcpSocket::disconnected, this, &ServerSocket::on_socket_disconnect);
    }
}

void ServerSocket::on_new_connection()
{
    emit log_server_message("New connection established");
}

void ServerSocket::read_socket()
{
    emit log_server_message("> read_socket...");

    if (m_socket) {

        QByteArray msg_buffer;
        QDataStream socket_stream(m_socket.get());
        socket_stream.setVersion(QDataStream::Qt_6_3);

        socket_stream.startTransaction();
        socket_stream >> msg_buffer;

        if (!socket_stream.commitTransaction()) {
            emit log_server_message("Waiting for more data...");
            return;
        }

        int MESSAGE   = 0;
        int SERVER_IP = 3;
        int PORT_NO   = 4;

        QString header = msg_buffer.mid(0, 128);
        QStringList header_sections = header.split(",");

        emit log_server_message(header);

        emit log_server_message(header_sections[SERVER_IP]);
        emit log_server_message(header_sections[PORT_NO]);

        QString file_type = header_sections[MESSAGE].split(":")[1];

        //QString file_type = header.split(",")[0].split(":")[1];
        msg_buffer = msg_buffer.mid(128);

        QString ftype = QString("FileType: %1").arg(file_type);

        emit log_server_message(ftype);

        if (file_type == "message") {
            RequestSender rs;
            rs.ipaddress = header_sections[SERVER_IP].split(":")[1];
            rs.port = header_sections[PORT_NO].split(":")[1].toInt();
            receive_request(rs, msg_buffer);
        }

        /*
        emit log_server_message("> socket is ready...");
        QByteArray json_data_bytes = m_socket->readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(json_data_bytes);

        qDebug() << json_doc;

        // QJsonObject json_data = json_doc.object();

        QString data_str = json_doc.toJson(QJsonDocument::Compact);
        emit log_server_message(data_str);
       */
    }
}

void ServerSocket::receive_request(RequestSender sender, QByteArray raw_request)
{
    QJsonDocument request = QJsonDocument::fromJson(raw_request);
    emit log_server_message(request.toJson());

    auto response = m_req_resp_manager->process_request(request);

}

void ServerSocket::on_socket_disconnect()
{
    m_socket->deleteLater();
    m_socket = nullptr;
}
