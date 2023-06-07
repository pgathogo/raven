#include "accesscontroller.h"
#include "../framework/databasemanager.h"

AccessController::AccessController()
    :mCanAccess{false},
      mDBManager{nullptr}
{
    mDBManager = new PostgresDatabaseManager();
}

AccessController::~AccessController()
{
    delete mDBManager;
}

bool AccessController::canAccess()
{
    return mCanAccess;
}

void AccessController::setCanAccess(bool val)
{
    mCanAccess = val;
}

AccessMap AccessController::accessMap()
{
    return mAccessMap;
}

void AccessController::setAccessMap(AccessMap access_map)
{
    mAccessMap = access_map;
}

SECURITY::User AccessController::user()
{
}

void AccessController::setUser(SECURITY::User)
{
}

//const std::unique_ptr<PostgresDatabaseManager> &AccessController::dbManager()
PostgresDatabaseManager* AccessController::dbManager()
{
    return mDBManager;
}
