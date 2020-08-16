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

    if (mInstance == nullptr){
        mInstance = new PostgresConnector(conninfo);
    }
    return mInstance;
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

