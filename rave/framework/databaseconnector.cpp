#include <QDebug>
#include "databaseconnector.h"
#include "ravenexception.h"

DatabaseConnector::DatabaseConnector()
{
}

DatabaseConnector::~DatabaseConnector()
{
}

/* ---- PostgresConnector ------ */

PostgresConnector* PostgresConnector::mInstance = nullptr;

PostgresConnector::PostgresConnector(std::string conninfo)
    :DatabaseConnector{}
    ,mConnInfo{conninfo}
    ,mConnection{nullptr}
{
    openConnection(conninfo);
    qDebug() << "Connection to DB opened.";
}

PostgresConnector* PostgresConnector::instance(const std::string conninfo)
{
    qDebug() << " Getting instance >>>>>>> ";

    if (mInstance == nullptr){
        qDebug() << "PostgresConnector::instances = nullptr *";
        mInstance = new PostgresConnector(conninfo);
    }
    return mInstance;
}

void PostgresConnector::test_connection(const std::string conninfo)
{
    //openConnection(conninfo);
    //qDebug() << "Testing db connection ...";
    auto connection = PQconnectdb(conninfo.c_str());
    if (PQstatus(connection) != CONNECTION_OK){
        std::string errMsg =  "Connection to database failed! - ";
        errMsg += PQerrorMessage(connection);
        PQfinish(connection);
        throw PostgresException("CONNECT", errMsg);
    }
}

void PostgresConnector::nullify_instance()
{
    mInstance = nullptr;
}

PostgresConnector::~PostgresConnector()
{
    PQfinish(mConnection);
}

void PostgresConnector::openConnection(const std::string conninfo)
{

    mConnection = PQconnectdb(conninfo.c_str());
    if (PQstatus(mConnection) != CONNECTION_OK){
        std::string errMsg =  "Connection to database failed! - ";
        errMsg += PQerrorMessage(mConnection);
        PQfinish(mConnection);
        throw PostgresException("CONNECT", errMsg);
    }

}

PGconn* PostgresConnector::connection()
{
    return mConnection;
}

