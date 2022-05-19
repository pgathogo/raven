#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "user.h"
#include "../framework/entitydatamodel.h"
#include "accesscontroller.h"

using AccessMap = std::map<std::string, std::string>;

class Authentication
{
public:
    Authentication();
    ~Authentication();
    void connect(const std::string uname, const std::string pword);
    void connect_cluster(const std::string, const std::string);
    void access_controller(const std::string uname);
    std::tuple<std::string, std::string> get_user_details(const std::string uname);
    PostgresDatabaseManager* dbManager();
    AccessMap accessMap();

    static AccessMap mAccessMap;
private:
    std::unique_ptr<EntityDataModel> mEdm;
    User mUser;
    PostgresDatabaseManager* mDBManager;
};

#endif // AUTHENTICATION_H
