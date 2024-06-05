#ifndef REQUESTRESPONSEMANAGER_H
#define REQUESTRESPONSEMANAGER_H

#include <variant>
#include <QObject>
#include <QJsonDocument>

#include "request_response_handler.h"

using Response = QJsonDocument;
using Request = QJsonDocument;

namespace NETWORK
{

    class RequestResponseManager : public QObject
    {
        Q_OBJECT

    public:
        RequestResponseManager();
        Response process_message(Request);

        int size();

        template<typename T>
        void register_handler()
        {
        auto t = T();
        m_handlers[t.handler_type()] = t;
        }

    signals:
        void log_man_message(const QString);

    private:
        std::map<QString, std::variant<DiskListRequestHandler>> m_handlers;

    };

}

#endif // REQUESTRESPONSEMANAGER_H
