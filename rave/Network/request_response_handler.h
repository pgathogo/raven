#ifndef REQUEST_RESPONSE_HANDLER_H
#define REQUEST_RESPONSE_HANDLER_H

#include <map>
#include <variant>

#include <QJsonDocument>

using Response = QJsonDocument;
using Request = QJsonDocument;

namespace NETWORK
{

    class BaseRequestResponseHandler
    {
    public:
        virtual QString handler_type() = 0;
        virtual Response handle_request(Request) = 0;
    };

    struct DiskInfo {
        double total{0.0};
        double free{0.0};
    };

    class DiskListRequestHandler : public BaseRequestResponseHandler
    {
    public:

        DiskListRequestHandler();
        QString handler_type() override;
        Response handle_request(Request) override;

        DiskInfo disk_info(const QString&);


    };

}

#endif  // REQUEST_REPONSE_HANDLER_H
