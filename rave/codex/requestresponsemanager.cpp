#include "requestresponsemanager.h"

RequestResponseManager::RequestResponseManager()
{
    register_handler<DiskListRequest>();
}

int RequestResponseManager::size()
{
    return m_handlers.size();
}


Response RequestResponseManager::process_request(Request request)
{
    QString req_str(request.toJson());

    req_str = "REQUEST: "+req_str;

    emit log_man_message(req_str);

    for (auto& [handler_type, handler] : m_handlers) {

        emit log_man_message("Handler Type: "+handler_type);

        if (request["type"] == handler_type){
            emit log_man_message("Handler found *");
            Response response = std::get<0>(handler).handle_request(request);
            return response;
        }

    }

    return QJsonDocument();

}
