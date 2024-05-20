#include "requestresponsemanager.h"

RequestResponseManager::RequestResponseManager()
{
    register_handler<DiskListRequestHandler>();
}

int RequestResponseManager::size()
{
    return m_handlers.size();
}


Response RequestResponseManager::process_request(Request request)
{
    for (auto& [handler_type, handler] : m_handlers) {

        emit log_man_message("Handler Type: "+handler_type);

        if (request["type"] == handler_type){
            Response response = std::get<0>(handler).handle_request(request);
            return response;
        }

    }

    return QJsonDocument();

}
