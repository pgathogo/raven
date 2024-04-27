#include <map>
#include <variant>

#include <QJsonDocument>

using Response = QJsonDocument;
using Request = QJsonDocument;

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

class DiskListRequest : public BaseRequestResponseHandler
{
public:

    DiskListRequest();
    QString handler_type() override;
    Response handle_request(Request) override;

    DiskInfo disk_info(const QString&);


};



