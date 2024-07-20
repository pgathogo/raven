#include "message.h"


namespace NETWORK {

Message::Message(){}

    Message::Message(const SenderIPAddress sender_ipaddress, SenderPort sender_port,
             MessageContent msg_content, RequireResponse require_response)
        :m_sender_ipaddress{sender_ipaddress}
        ,m_sender_port{sender_port}
        ,m_requires_response{require_response}
    {
        m_message_data.setObject(msg_content);
    }

    Message::Message(const Message& other)
    {
        m_message_data = other.m_message_data;
        m_sender_ipaddress = other.m_sender_ipaddress;
        m_sender_port = other.m_sender_port;
        m_requires_response = other.m_requires_response;
        m_message_type = other.m_message_type;
        m_message_response =  other.m_message_response;
    }

    void Message::set_message_response(QJsonDocument response)
    {
        m_message_response = response;
    }

    void Message::test_emit(const QString msg)
    {
        emit log_message(msg);
    }

    QDataStream& operator<<(QDataStream& ds, Message& msg)
    {
        emit msg.log_message("<<operator<< ");

        QByteArray header;

        QString header_str = QString("ft:message;fn:null;fs:0;sip:%1;port:%2;")
             .arg(msg.m_sender_ipaddress)
             .arg(QString::number(msg.m_sender_port));

        header.prepend(header_str.toUtf8().constData());

        //header.resize(header_str.size()+5);

        QByteArray byte_array = msg.m_message_data.toJson();
        byte_array.prepend(header);

        //emit msg.log_message(QString(byte_array));
        qDebug() << QString(byte_array);

        return ds << byte_array;
    }

    QDataStream& operator>>(QDataStream& ds, Message& msg)
    {

        emit msg.log_message("Operator >> ");

        QByteArray msg_buffer;

        ds >> msg_buffer;

        emit msg.log_message(QString(msg_buffer));


        int MESSAGE   = 0;
        int SERVER_IP = 3;
        int PORT_NO   = 4;
        int DATA      = 5;

        QString header = msg_buffer.mid(0, 128);


        QStringList header_sections = header.split(";");

        QString file_type = header_sections[MESSAGE].split(":")[1];

        msg_buffer = msg_buffer.mid(128);

        qDebug() << QString(msg_buffer);

        msg.m_message_type = file_type;
        msg.m_sender_ipaddress = header_sections[SERVER_IP];
        msg.m_sender_port= header_sections[PORT_NO].toInt();

        std::string s = header_sections[DATA].toStdString();

        emit msg.log_message("XXXXX :");
        emit msg.log_message(QString::fromStdString(s));


        //QJsonDocument resp_required = QJsonDocument::fromJson(header_sections[RESP_REQUIRED].toStdString());

        //msg.m_requires_response = resp_required["require_response"].toInt() == 0 ? RequireResponse::No : RequireResponse::Yes;
        //QString jstring = QString::fromStdString(s);
        QByteArray ba = QByteArray::fromStdString(s);
        emit msg.log_message("AAAA");
        QJsonDocument jd = QJsonDocument::fromJson(ba);

        if (jd.isNull()) {
            emit msg.log_message("JD is null");
            return ds;
        }

        if (!jd.isObject()) {
            emit msg.log_message("JD is not an Object");
            return ds;
        }

        QJsonObject jo = jd.object();

        if (jo.isEmpty()) {
            emit msg.log_message("Object is Empty");
            return ds;
        }

        QVariantMap vm = jo.toVariantMap();

        emit msg.log_message(vm["type"].toString());

        msg.m_request = vm["type"].toString();
        msg.m_need_response = vm["require_response"].toInt();


        //msg.m_message_data = QJsonDocument::fromJson(ba);
        emit msg.log_message("BBB");


        //msg.m_requires_response = header_sections[RESP_REQUIRED].toInt() == 0 ? RequireResponse::No : RequireResponse::Yes;
        //msg.m_message_data = QJsonDocument();

        emit msg.log_message(msg.m_request);
        emit msg.log_message(QString::number(msg.m_need_response));

        emit msg.log_message("CCC");
        // auto jdoc = QJsonDocument(jo);
        //msg.m_message_data = jdoc;

        return ds;
    }


}
