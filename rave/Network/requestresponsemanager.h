#ifndef REQUESTRESPONSEMANAGER_H
#define REQUESTRESPONSEMANAGER_H

#include <variant>
#include <QObject>
#include <QJsonDocument>

#include "request_response_handler.h"

using Response = QJsonDocument;
//using Request = QString;

namespace NETWORK
{

    class RequestResponseManager : public QObject
    {
        Q_OBJECT

    public:
        RequestResponseManager();
        Response process_message(QString);

        int size();

        template<typename T>
        void register_handler()
        {
            if constexpr(std::is_base_of<BaseRequestResponseHandler, T>::value) {
                std::shared_ptr<T> t = std::make_shared<T>();
                m_handlers[t->handler_type()] = t;
            }
        }

        template<typename T>
        QJsonDocument pm(T t, QString req)
        {
            if constexpr(std::is_base_of<BaseRequestResponseHandler, T>::value) {
                return t.handle_request(req);
            }

        }

    private slots:
        void log_handler_message(const QString);

    signals:
        void log_man_message(const QString);

    private:
        std::map<QString, std::variant<std::shared_ptr<DiskListRequestHandler>>> m_handlers;

        void connect_handler_logging();

    };

}

#endif // REQUESTRESPONSEMANAGER_H
