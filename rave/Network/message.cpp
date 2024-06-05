#include "message.h"


namespace NETWORK {

Message::Message(){}

    Message::Message(SenderIPAddress sender_ipaddress, SenderPort sender_port,
             MessageContent msg_content, RequireResponse require_response)
        :m_sender_ipaddress{sender_ipaddress}
        ,m_sender_port{sender_port}
        ,m_requires_response{require_response}
    {
        m_message_data.setObject(msg_content);
    }

    QString Message::message_type()
    {
        return m_message_type;
    }

    QJsonDocument Message::message_data()
    {
        return m_message_data;
    }

    void Message::set_message_response(QJsonDocument response)
    {
        m_message_response = response;
    }

    QJsonDocument Message::message_response()
    {
        return m_message_response;
    }

    QDataStream& operator<<(QDataStream& ds, const Message& msg)
    {
        QByteArray header;

        QString header_str = QString("filetype:message,fileName:null,fileSize:0,serverip:%1,port:%2,;")
             .arg(msg.m_sender_ipaddress)
             .arg(QString::number(msg.m_sender_port));
        header.prepend(header_str.toUtf8().constData());
        header.resize(128);

        QByteArray byte_array = msg.m_message_data.toJson();
        byte_array.prepend(header);

        return ds << byte_array;
    }

    QDataStream& operator>>(QDataStream& ds, Message& msg)
    {
        QByteArray msg_buffer;
        ds >> msg_buffer;

        int MESSAGE   = 0;
        int SERVER_IP = 3;
        int PORT_NO   = 4;
        int RESP_REQUIRED = 5;

        QString header = msg_buffer.mid(0, 128);
        QStringList header_sections = header.split(",");

        QString file_type = header_sections[MESSAGE].split(":")[1];

        msg_buffer = msg_buffer.mid(128);

        msg.m_message_type = file_type;
        msg.m_sender_ipaddress = header_sections[SERVER_IP];
        msg.m_sender_port= header_sections[PORT_NO].toInt();
        msg.m_requires_response = header_sections[RESP_REQUIRED].toInt() == 0 ? RequireResponse::No : RequireResponse::Yes;
        msg.m_message_data = QJsonDocument::fromJson(msg_buffer.mid(128));

        return ds;
    }




}
