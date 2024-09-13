#include "requestresponsemanager.h"

namespace NETWORK
{
    RequestResponseManager::RequestResponseManager()
    {
        register_handler<DiskListRequestHandler>();

        connect_handler_logging();
    }

    void RequestResponseManager::connect_handler_logging()
    {
        /*
        for(auto& [handler_type, handler] : m_handlers)
        {
            connect(&std::get<0>(handler), &BaseRequestResponseHandler::log_handler_message,
                    this, &RequestResponseManager::log_handler_message);
        }
   */


    }

    int RequestResponseManager::size()
    {
        return m_handlers.size();
    }


    Response RequestResponseManager::process_message(QString request)
    {
        for (auto& [handler_type, handler] : m_handlers) {

        emit log_man_message("Handler Type: "+handler_type);

        emit log_man_message("Request Type : "+ request);

        if (request == handler_type) {
            emit log_man_message("Handling request...");

            //Response response = std::get<0>(handler).handle_request(request);
            Response response = pm<decltype(std::get<0>(handler))>(std::get<0>(handler), request);
            return response;
          }

        }

        return QJsonDocument();

    }
    void RequestResponseManager::log_handler_message(const QString msg)
    {
        emit log_man_message(msg);
    }

}
