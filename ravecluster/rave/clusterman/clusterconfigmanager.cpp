#include <tuple>
#include "clusterconfigmanager.h"
#include "station.h"
#include "server.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"

namespace ClusterManager
{

    ClusterConfigurationManager::ClusterConfigurationManager()
    {

    }

    std::vector<std::shared_ptr<ClusterManager::Station>> ClusterConfigurationManager::stations(int cluster_id)
    {
        std::vector<std::shared_ptr<ClusterManager::Station>> stns;

        ClusterManager::Station stn;

        EntityDataModel edm(std::make_unique<ClusterManager::Station>());
        auto cluster_filter = std::make_tuple(stn.cluster()->dbColumnName(), "=", cluster_id);
        std::string filter = edm.prepareFilter(cluster_filter);
        edm.search(filter);

        for (auto const& [name, entity]: edm.modelEntities()){
            auto stnptr = dynamic_cast<ClusterManager::Station*>(entity.get());
            auto station = std::make_shared<ClusterManager::Station>();

            *station = *stnptr;
            stns.push_back(std::move(station));
        }

        return std::move(stns);
    }

    std::vector<std::shared_ptr<ClusterManager::Server>> ClusterConfigurationManager::servers(ClusterId cluster_id, std::string server_type)
    {
        std::vector<std::shared_ptr<ClusterManager::Server>> svrs;

        ClusterManager::Server svr;

        EntityDataModel edm(std::make_unique<ClusterManager::Server>());
        auto cluster_filter = std::make_tuple(svr.cluster()->dbColumnName(), "=", cluster_id);

        std::tuple<std::string, std::string, std::string> server_filter = std::make_tuple("1", "=","1");
        if (!server_type.empty()){
            server_filter = std::make_tuple(svr.server_type()->dbColumnName(), "=", server_type);
        }

        std::string filter = edm.prepareFilter(cluster_filter, server_filter);

        edm.search(filter);

        for (auto const& [name, entity]: edm.modelEntities()){
            auto svrptr = dynamic_cast<ClusterManager::Server*>(entity.get());

            auto server = std::make_shared<ClusterManager::Server>();
            *server = *svrptr;

            svrs.push_back(std::move(server));
        }

        return std::move(svrs);
    }

}
