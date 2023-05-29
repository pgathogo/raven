#ifndef ACCESSCONTROLLER_H
#define ACCESSCONTROLLER_H

#include <map>
#include "user.h"

using AccessMap = std::map<std::string, std::string>;

class PostgresDatabaseManager;

class AccessController
{
public:
    AccessController();
    ~AccessController();

    bool canAccess();
    void setCanAccess(bool val);

    AccessMap accessMap();
    void setAccessMap(AccessMap);

    SECURITY::User user();
    void setUser(SECURITY::User u);

    //std::unique_ptr<PostgresDatabaseManager> const& dbManager();
    PostgresDatabaseManager* dbManager();

private:
    SECURITY::User mUser;
    bool mCanAccess;
    AccessMap mAccessMap;
    //std::unique_ptr<PostgresDatabaseManager> mDBManager;
    PostgresDatabaseManager* mDBManager;
};

#endif // ACCESSCONTROLLER_H
