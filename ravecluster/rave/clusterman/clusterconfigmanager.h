#ifndef CLUSTERMANAGER_H
#define CLUSTERMANAGER_H

#include <vector>
#include <memory>
#include <string>

#include "../framework/entitydatamodel.h"

namespace SECURITY{
class User;
}

namespace ClusterManager {

class Server;

using ClusterId = int;
using ServerType = std::string;
using ClusterServers = std::vector<std::shared_ptr<ClusterManager::Server>>;

class Station;
class Server;

    class ClusterConfigurationManager
    {
    public:
        ClusterConfigurationManager();

        // Return list of stations for a given cluster
        std::vector<std::shared_ptr<ClusterManager::Station>> stations(ClusterId);

        std::vector<std::shared_ptr<ClusterManager::Server>> servers(ClusterId, std::string server_type="");

        bool grant_user_station_access(std::string, int);
        std::unique_ptr<ClusterManager::Station> station(int);
        ClusterServers active_servers_by_db(ClusterServers, std::string);
        bool create_user(SECURITY::User*);
        std::string user_table_privileges(std::string);
        std::string user_sequence_privileges(std::string);
        void flag_password_for_reset(std::string);
        void alter_cluster_user(std::string, std::string);
    private:
        void fetch_table_sequences(EntityDataModel&, std::vector<std::string>&);

    };

}

#endif // CLUSTERMANAGER_H
