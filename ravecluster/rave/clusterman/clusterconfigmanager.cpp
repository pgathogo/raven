#include <tuple>
#include "clusterconfigmanager.h"
#include "station.h"
#include "server.h"
#include "../framework/choicefield.h"
#include "../framework/ravenexception.h"

#include "../security/authentication.h"
#include "../security/structs.h"
#include "../security/user.h"

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

        for (auto const& [name, entity]: edm.modelEntities())
        {
            auto svrptr = dynamic_cast<ClusterManager::Server*>(entity.get());

            auto server = std::make_shared<ClusterManager::Server>();
            *server = *svrptr;

            svrs.push_back(std::move(server));
        }

        return std::move(svrs);
    }

    bool ClusterConfigurationManager::grant_user_station_access(std::string username, int station_id)
    {
        auto stn = station(station_id);

        std::string db_name = stn->db_name()->value();

        auto db_servers = servers(stn->cluster()->value(), "DBS");

        auto act_svrs = active_servers_by_db(std::move(db_servers), db_name);

        if (act_svrs.size() == 0)
            return false;

        for (auto& server : act_svrs)
        {
            ConnInfo ci;
            ci.db_name = db_name;
            ci.host = server->server_ip()->value();
            ci.port = server->port_no()->value();
            ci.username = server->db_admin()->value();
            ci.password = decrypt_str(server->db_admin_password()->value());

            // TODO:: Add logging
            std::cout << "DB Name: "<< ci.db_name << '\n';
            std::cout << "Host: "<< ci.host << '\n';
            std::cout << "Port: "<< ci.port << '\n';
            std::cout << "Username: "<< ci.username << '\n';
            std::cout << "Password: "<< ci.password << '\n';


            Authentication auth(ci);

            EntityDataModel edm(auth);

            auto user = std::make_unique<SECURITY::User>();

            user->set_role_name(username);
            user->set_rol_password("abc123");

            // TODO: Build a better abstraction for making create statements
            /* --------------------------------------------------------- */

            EntityDataModel user_edm(auth);
            std::string create_stmt = user->make_create_user_stmt();
            std::cout << create_stmt << '\n';

            try{
                user_edm.executeRawSQL(create_stmt);
                std::string msg = std::format("User created in server `{}`", ci.host);
                qDebug() << stoq(msg);
            } catch(DatabaseException& de){
                qDebug() << "Error creating user: \n"
                         << stoq(de.errorMessage());
            }
              //  -- Sequence privileges ---

            EntityDataModel seq_edm(auth);
            std::string seq_priv = user_sequence_privileges(username);
            try{
                seq_edm.executeRawSQL(seq_priv);
                std::string seq_msg = std::format("Sequence privileges granted to user `{}`", username);
                qDebug() << stoq(seq_msg);
            }catch(DatabaseException& de){
                qDebug() << "Error granting sequence privileges: \n"
                         << stoq(de.errorMessage());
            }

              // Grant table privileges
            EntityDataModel tab_edm(auth);
            std::string user_priv = user_table_privileges(username);
            try{
                tab_edm.executeRawSQL(user_priv);
                std::string priv_msg = std::format("Table privileges granted to user `{}`", username);
                qDebug() << stoq(priv_msg);
            } catch (DatabaseException& de) {
                qDebug() << "Error granting privileges: \n"
                         << stoq(de.errorMessage());
            }

            /* ---------------------------------------------------------- */

        }
    }


    ClusterServers ClusterConfigurationManager::active_servers_by_db(ClusterServers servers, std::string db_name)
    {
        ClusterServers active_svrs;

        ConnInfo ci;

        for(auto& server : servers)
        {
            ci.db_name = db_name;
            ci.host = server->server_ip()->value();
            ci.port = server->port_no()->value();
            ci.username = server->db_admin()->value();
            ci.password = decrypt_str(server->db_admin_password()->value());

            try{
                Authentication::test_connection(ci);
                active_svrs.push_back(std::move(server));
            } catch(PostgresException& pa) {
                qDebug() << "active_servers_by_db:: Server *NOT* available";
            }
        }

        return std::move(active_svrs);
    }

    std::unique_ptr<ClusterManager::Station> ClusterConfigurationManager::station(int station_id)
    {
        EntityDataModel edm(std::make_unique<ClusterManager::Station>());
        ClusterManager::Station st;
        auto station_filter = std::make_tuple("id", "=", station_id);

        std::string filter = edm.prepareFilter(station_filter);

        edm.search(filter);

        if (edm.modelEntities().size() == 0)
            return nullptr;

        auto const& [name, entity] =  edm.modelEntities()[0];

        auto stptr = dynamic_cast<ClusterManager::Station*>(entity.get());
        auto station = std::make_unique<ClusterManager::Station>();

        *station = *stptr;

        return std::move(station);

    }

    void ClusterConfigurationManager::fetch_table_sequences(EntityDataModel& edm,
                                                            std::vector<std::string>& tseq)
    {
       std::stringstream sql;
       sql << "SELECT sequence_name FROM information_schema.sequences "
           << " WHERE sequence_name ~* 'rave_'; ";

       edm.readRaw(sql.str());

       auto cache = edm.getDBManager()->provider()->cache();
       cache->first();
       do{
           auto elem = cache->currentElement();

           auto it = elem->begin();
           tseq.push_back((*it).second);

           cache->next();

       }while(!cache->isLast());

    }

    bool ClusterConfigurationManager::create_user(SECURITY::User* user)
    {
       EntityDataModel edm;
       std::string create_stmt = user->make_create_user_stmt();
       edm.executeRawSQL(create_stmt);
    }

    std::string ClusterConfigurationManager::user_table_privileges(std::string username)
    {
       std::stringstream sql;

       sql << "DO $$ "
           << "DECLARE tab_name text; "
           << "BEGIN "
           << " FOR tab_name IN ( SELECT table_name FROM information_schema.tables WHERE table_name ~* 'rave_') "
           << "LOOP";

       std::string exec = std::format(" EXECUTE 'GRANT ALL ON TABLE public.'||tab_name||' TO {} ';", username);

       sql << exec
           << " END LOOP;"
           << " END $$;";

       return sql.str();

    }

    std::string ClusterConfigurationManager::user_sequence_privileges(std::string username)
    {
       std::stringstream sql;

       sql << "DO $$ "
           << "DECLARE seq_name text; "
           << "BEGIN "
           << " FOR seq_name IN ( SELECT sequence_name FROM information_schema.sequences WHERE sequence_name ~* 'rave_') "
           << "LOOP";

       std::string exec = std::format(" EXECUTE 'GRANT ALL ON sequence public.'||seq_name||' TO {} ';", username);

       sql << exec
           << " END LOOP;"
           << " END $$;";

       return sql.str();

    }

    void ClusterConfigurationManager::flag_password_for_reset(std::string username)
    {
       std::string sql = std::format("Select * from rave_userconfig where username = '{}'", username);
       EntityDataModel sel_edm;
       int count = sel_edm.readRaw(sql);

       if (count > 0){
           std::string upd_sql = std::format("Update rave_userconfig set reset_password = 1 where username = '{}' ", username);
           EntityDataModel upd_edm;
           upd_edm.executeRawSQL(upd_sql);
       } else {
           std::string ins_sql = std::format("Insert into rave_userconfig (username, reset_password) Values ('{}', 1)", username);
           EntityDataModel ins_edm;
           ins_edm.executeRawSQL(ins_sql);
       }
    }

    void ClusterConfigurationManager::alter_cluster_user(std::string username, std::string password)
    {
        std::string alter_stmt = std::format("Alter user {} with password '{}' valid until '{}'",
                                             username,
                                             password,
                                             "infinity");

       EntityDataModel edm;
       edm.executeRawSQL(alter_stmt);

    }



}
