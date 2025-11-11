#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "user.h"
#include "accesscontroller.h"
#include "structs.h"

using AccessMap = std::map<std::string, std::string>;

class PostgresDatabaseManager;

class Authentication
{
public:
    Authentication();
    Authentication(ConnInfo);
    ~Authentication();
    void connect(const std::string uname, const std::string pword);
    void connect_to_cluster_server(const std::string, const std::string);
    void access_controller(const std::string uname);
    std::tuple<std::string, std::string> get_user_details(const std::string uname);
    PostgresDatabaseManager* dbManager();
    AccessMap accessMap();
    std::unique_ptr<PostgresDatabaseManager> connect_to_server();
    void connect_to_station(ConnInfo);
    ConnInfo cluster_server_conninfo();
    StationInfo station_by_station_code(const QString);

    static void test_connection(ConnInfo&);

    static AccessMap mAccessMap;
private:
    SECURITY::User mUser;
    PostgresDatabaseManager* mDBManager;
    ConnInfo m_conn_info;
};

#endif // AUTHENTICATION_H
