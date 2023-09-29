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

    class ClusterController
    {
    public:
        ClusterController();

        // Return list of stations for a given cluster
        std::vector<std::shared_ptr<ClusterManager::Station>> stations(ClusterId);
        bool grant_access(std::string);
        bool alter_password(std::string, std::string);
        void remove_password_reset_flag(std::string);

        std::vector<std::shared_ptr<ClusterManager::Server>> servers(ClusterId, std::string server_type="");

        bool grant_user_station_access(std::string, int);

        bool grant_user_table_access(std::string);
        bool table_access(std::string, int);

        std::unique_ptr<ClusterManager::Station> station(int);
        ClusterServers active_servers_by_db(ClusterServers, std::string);
        bool create_user(SECURITY::User*);
        std::string grant_table_privileges_stmt(std::string);
        std::string grant_sequence_privileges_stmt(std::string);
        void flag_password_for_reset(std::string);
        void alter_cluster_user(std::string, std::string);
        void alter_password_cluster_server(std::string, std::string);
    private:
        void fetch_table_sequences(EntityDataModel&, std::vector<std::string>&);
        void alter_user_password(const ClusterManager::Server*, const std::string,
                                 const std::string);

    };

}

#endif // CLUSTERMANAGER_H
