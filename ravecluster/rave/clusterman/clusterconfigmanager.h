#ifndef CLUSTERMANAGER_H
#define CLUSTERMANAGER_H

#include <vector>
#include <memory>
#include <string>

namespace ClusterManager {

using ClusterId = int;
using ServerType = std::string;

class Station;
class Server;


    class ClusterConfigurationManager
    {
    public:
        ClusterConfigurationManager();

        // Return list of stations for a given cluster
        std::vector<std::shared_ptr<ClusterManager::Station>> stations(ClusterId);
        std::vector<std::shared_ptr<ClusterManager::Server>> servers(ClusterId, std::string server_type="");

    };


}

#endif // CLUSTERMANAGER_H
