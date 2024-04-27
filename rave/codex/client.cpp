
#include <QTcpSocket>

#include <QJsonObject>
#include <QJsonDocument>

#include "client.h"

const QString DRIVE_LIST = "drive-list";

Client::Client(QObject* parent)
    : QObject{parent}
{
    m_req_resp_manager = std::make_unique<RequestResponseManager>();

    connect(m_req_resp_manager.get(), &RequestResponseManager::log_man_message, this, &Client::show_man_message);

    make_client_socket();

    //register_request_handlers();
}

void Client::make_client_socket()
{
    // Read socket details from the registry
    m_socket = std::make_unique<QTcpSocket>(this);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &Client::read_socket);

    connect(m_socket.get(), &QTcpSocket::connected, this, [this]{
        emit log_message("Client connected.");
    });

    connect(m_socket.get(), &QTcpSocket::disconnected, this, [this]{
        emit log_message("Client disconnected");
    });
}

void Client::read_socket()
{
    emit log_message("Attempting to read socket...");

    QByteArray buffer;

    QDataStream socket_stream(m_socket.get());
    socket_stream.setVersion(QDataStream::Qt_6_3);

    socket_stream.startTransaction();
    socket_stream >> buffer;

    if (!socket_stream.commitTransaction() )
    {
        QString msg = QString("%1: Waiting for more data...");
        emit log_message(msg);
        return;
    }

    QString header = buffer.mid(0, 128);
    QString file_type = header.split(",")[0].split(":")[1];
    buffer = buffer.mid(128);

    //TODO: Handle `Attachements`

    if (file_type == "message"){
        receive_request(buffer);
    }

}

void Client::receive_request(QByteArray raw_request)
{
    QJsonDocument request = QJsonDocument::fromJson(raw_request);
    auto response = m_req_resp_manager->process_request(request);
}


void Client::soc_connect()
{

    emit log_message("Connected");
}

void Client::soc_disconnect()
{
    emit log_message("Disconnected");
}

void Client::show_man_message(const QString msg)
{
    emit log_message(msg);
}

void Client::request(QString req)
{

    QJsonObject request;
    request["type"] = req;
    QJsonDocument json(request);

    Response response = m_req_resp_manager->process_request(json);

    QString resp_str = response.toJson();
    emit log_message(resp_str);
}
