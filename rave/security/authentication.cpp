#include <sstream>

#include <QSettings>

#include "authentication.h"
#include "user.h"
#include "../framework/databasemanager.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h"

AccessMap Authentication::mAccessMap;

Authentication::Authentication()
    :mUser{}
    ,mDBManager{nullptr}
{
}

Authentication::Authentication(ConnInfo ci)
    :m_conn_info{ci}
    ,mDBManager{nullptr}
{
}

Authentication::~Authentication()
{
    if (mDBManager != nullptr)
       delete mDBManager;
}

void Authentication::connect(const std::string uname, const std::string pword)
{
    QString dbname{};
    QString server{};
    int port=-1;

    QSettings registry(R"(HKEY_LOCAL_MACHINE\SOFTWARE\PMSL\Raven\Database)", QSettings::NativeFormat);
    if (registry.childKeys().contains("dbname", Qt::CaseInsensitive))
        dbname = registry.value("dbname").toString();

    if (registry.childKeys().contains("server", Qt::CaseInsensitive))
        server = registry.value("server").toString();

    if (registry.childKeys().contains("port", Qt::CaseInsensitive))
        port = registry.value("port").toInt();

    std::string conninfo = "user="+uname+" password="+pword+" dbname="+dbname.toStdString()+" port="+std::to_string(port);

    if (mDBManager != nullptr){
        //delete mDBManager;
       mDBManager = new PostgresDatabaseManager(conninfo, true);

    } else {
        mDBManager = new PostgresDatabaseManager(conninfo);
    }

}

void Authentication::connect_to_station(ConnInfo ci)
{
    std::string conninfo = format("host={} user={} password={} dbname={} port={}",
                                  ci.host, ci.username,
                                  ci.password, ci.db_name,
                                  std::to_string(ci.port));

    mDBManager = new PostgresDatabaseManager(conninfo, false);
}


std::unique_ptr<PostgresDatabaseManager> Authentication::connect_to_server()
{
    std::string conninfo = format("host={} user={} password={} dbname={} port={}", m_conn_info.host, m_conn_info.username,
                                  m_conn_info.password, m_conn_info.db_name, std::to_string(m_conn_info.port));

    try{
        Authentication::test_connection(m_conn_info);
        auto pdm = std::make_unique<PostgresDatabaseManager>(conninfo, true);
        return(std::move(pdm));
    }catch (DatabaseException& de){
        throw;
    }

}

void Authentication::test_connection(ConnInfo& ci)
{
    std::string conninfo = format("host={} user={} password={} dbname={} port={}", ci.host, ci.username,
                                  ci.password, ci.db_name, std::to_string(ci.port));

    try{
       PostgresDatabaseManager::test_connection(conninfo);
    }catch(DatabaseException& de){
       throw;
    }
}

ConnInfo Authentication::cluster_server_conninfo()
{
    QString dbname{};
    QString server{};
    QString password{};
    int port=-1;

    QSettings registry(R"(HKEY_LOCAL_MACHINE\SOFTWARE\PMSL\Raven\Database)", QSettings::NativeFormat);
    if (registry.childKeys().contains("clusterdb", Qt::CaseInsensitive))
        dbname = registry.value("clusterdb").toString();

    if (registry.childKeys().contains("clusterserver", Qt::CaseInsensitive))
        server = registry.value("clusterserver").toString();

    if (registry.childKeys().contains("password", Qt::CaseInsensitive))
        password = registry.value("password").toString();

    if (registry.childKeys().contains("clusterport", Qt::CaseInsensitive))
        port = registry.value("clusterport").toInt();

    ConnInfo ci;
    ci.db_name = dbname.toStdString();
    ci.host = server.toStdString();
    ci.port = port;
    ci.password = decrypt_str(password.toStdString());

    return ci;
}

void Authentication::connect_to_cluster_server(const std::string uname, const std::string pword)
{
    QString dbname{};
    QString server{};
    int port=-1;

    QSettings registry(R"(HKEY_LOCAL_MACHINE\SOFTWARE\PMSL\Raven\Database)", QSettings::NativeFormat);
    if (registry.childKeys().contains("clusterdb", Qt::CaseInsensitive))
        dbname = registry.value("clusterdb").toString();

    if (registry.childKeys().contains("clusterserver", Qt::CaseInsensitive))
        server = registry.value("clusterserver").toString();

    if (registry.childKeys().contains("clusterport", Qt::CaseInsensitive))
        port = registry.value("clusterport").toInt();

    std::string conninfo = "user="+uname+" password="+pword+" dbname="+dbname.toStdString()+" port="+std::to_string(port);

    ConnInfo ci;
    ci.host = server.toStdString();
    ci.port = port;
    ci.username = uname;
    ci.password = pword;
    ci.db_name = dbname.toStdString();

    qDebug() << "**** Testing connection ****";

    try{
        Authentication::test_connection(ci);
    }catch (DatabaseException& de){
        throw;
    }

    qDebug() << "**** Good connection... proceed";

    mDBManager = new PostgresDatabaseManager(conninfo);
}

void Authentication::access_controller(const std::string uname)
{
    auto[uid, validity] = get_user_details(uname);

    // TODO: check password validity

    std::stringstream ss;
    ss << "select rave_content.name, access_bit "
          <<" from rave_contentauth, pg_roles, rave_content "
          <<" where rave_content.id = parent_id "
          <<" and pg_roles.oid = detail_id "
          <<" and  detail_id = (select roleid from pg_auth_members "
          <<" where member = "+uid+")";
    std::string sql = ss.str();

    EntityDataModel edm;
    edm.readRaw(sql);

    std::string key;
    std::string value;
    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() > 0 ) {

        provider->cache()->first();
        do{
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();
            for(; itB!=itE; ++itB){
                if ((*itB).first == "name")
                    key = (*itB).second;
                if ((*itB).first == "access_bit")
                    value = (*itB).second;
            }
            mAccessMap[key] = value;
            provider->cache()->next();
        }while(!provider->cache()->isLast());
    }


}

std::tuple<std::string, std::string> Authentication::get_user_details(const std::string uname)
{
    std::unique_ptr<SECURITY::User> user = std::make_unique<SECURITY::User>();
    auto filter_username = std::make_tuple(
                "lower("+user->role_name()->dbColumnName()+")",
                "=",
                uname);
    EntityDataModel edm(std::move(user));

    std::string filter = edm.prepareFilter(filter_username);

    edm.search(filter);

    if (edm.count() == 0)
        return std::make_tuple("","");

    auto provider = edm.getDBManager()->provider();
    provider->cache()->first();
    auto record = provider->cache()->currentElement();
    auto recB = record->begin();
    auto recE = record->end();

    std::string uid;
    std::string validity;
    for(; recB != recE; ++recB){
        if ((*recB).first == "usesysid"){
            uid = (*recB).second;
        }
        if ((*recB).first == "valuntil"){
            validity = (*recB).second;
        }
    }

    auto result = std::make_tuple(uid, validity);
    return result;

}

PostgresDatabaseManager *Authentication::dbManager()
{
    return mDBManager;
}

AccessMap Authentication::accessMap()
{
    return mAccessMap;
}

