#include <QJsonObject>
#include <QJsonDocument>

#include "clientsocket.h"


ClientSocket::ClientSocket(QObject *parent)
    : QObject{parent}
{
   make_client_socket();
}

void ClientSocket::make_client_socket()
{
    m_socket = std::make_unique<QTcpSocket>(this);

    m_req_resp_manager = std::make_unique<RequestResponseManager>();

    connect(m_socket.get(), &QTcpSocket::readyRead, this, &ClientSocket::read_socket);

    connect(m_socket.get(), &QTcpSocket::connected, this, [this]{
        emit log_message("Connected to Server.");
    });

    connect(m_socket.get(), &QTcpSocket::disconnected, this, [this]{
        emit log_message("Disconnected from Server.");
    });

    m_socket->connectToHost(QHostAddress("127.0.0.1"), 12345);
    if(m_socket->waitForConnected())
        emit log_message("Connected to server.");
    else {
        emit log_message("Error connecting to server!");
    }

}

void ClientSocket::read_socket()
{
    emit log_message("Attempting to read socket...");

    QByteArray msg_buffer;

    QDataStream socket_stream(m_socket.get());
    socket_stream.setVersion(QDataStream::Qt_6_3);

    socket_stream.startTransaction();
    socket_stream >> msg_buffer;

    if (!socket_stream.commitTransaction() ) {
        QString msg = QString("%1: Waiting for more data...");
        emit log_message(msg);
        return;
    }

    QString header = msg_buffer.mid(0, 128);
    QString file_type = header.split(",")[0].split(":")[1];
    msg_buffer = msg_buffer.mid(128);

    if (file_type == "message") {
        receive_request(msg_buffer);
    }

}

void ClientSocket::receive_request(QByteArray raw_request)
{
    QJsonDocument request = QJsonDocument::fromJson(raw_request);
    auto response = m_req_resp_manager->process_request(request);
}

void ClientSocket::send_request(const QString req)
{
    QJsonObject request;
    request["type"] = req;
    QJsonDocument json(request);

    if (m_socket) {
        if (m_socket->isOpen())
        {
            QDataStream socket_stream(m_socket.get());
            socket_stream.setVersion(QDataStream::Qt_5_15);

            QByteArray header;
            header.prepend("filetype:message,fileName:null,fileSize:0,serverip:127.0.0.1,port:5544,;");
            header.resize(128);

            QByteArray byte_array = json.toJson();
            byte_array.prepend(header);

            socket_stream << byte_array;

            emit log_message("Request sent.");
        }
        else
            emit log_message("Socket not open!");
    }
    else
        emit log_message("Socket not connected!");

    /*
    QJsonDocument response = m_req_resp_manager->process_request(json);
    qDebug() << response;
    QString resp_str = response.toJson();
    emit log_message(resp_str);
    */

}

void ClientSocket::soc_connect()
{
    emit log_message("Connected.");
}

void ClientSocket::soc_disconnect()
{
    emit log_message("Disconnectd");
}
