#include <QDebug>
#include "databaseconnector.h"

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
}

PostgresConnector* PostgresConnector::instance(const std::string conninfo)
{
    // put a try catch!!

    if (mInstance == nullptr){
        mInstance = new PostgresConnector(conninfo);
    }
    return mInstance;
}

PostgresConnector::~PostgresConnector()
{
    PQfinish(mConnection);
}

bool PostgresConnector::openConnection(const std::string conninfo)
{
    mConnection = PQconnectdb(conninfo.c_str());
    if (PQstatus(mConnection) != CONNECTION_OK){
        qDebug() << "Connection to database failed! - " << PQerrorMessage(mConnection);
        PQfinish(mConnection);
        return false;
    }

    qDebug() << "Connection to DB opened. - " << PQerrorMessage(mConnection);
    return true;
}

PGconn* PostgresConnector::connection()
{
    return mConnection;
}
