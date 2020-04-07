#include <QDebug>
#include <map>
#include "dataprovider.h"
#include "queryset.h"

BaseDataProvider::BaseDataProvider()
{
}

BaseDataProvider::~BaseDataProvider()
{
}

void BaseDataProvider::append(StringMapped* data)
{
    dataQuerySet.addRecord(data);
}

int BaseDataProvider::cacheSize() const
{
    return dataQuerySet.elemSize();
}

std::vector<StringMapped*>::iterator BaseDataProvider::cacheIter()
{
    return dataQuerySet.elemIter();
}

DataQuerySet<StringMapped>* BaseDataProvider::cache()
{
    return &dataQuerySet;
}

PostgresDataProvider::PostgresDataProvider()
    : BaseDataProvider(),
      conn{nullptr}
{
    qDebug() << "Open connection ...";
    openConnection();
}

PostgresDataProvider::~PostgresDataProvider()
{
    PQfinish(conn);
}

bool PostgresDataProvider::openConnection()
{
    // read this values from registry
    const char* conninfo = "user=postgres password=abc123 dbname=raven port=5433";

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK){
        qDebug() << "Connection to database failed! - " << PQerrorMessage(conn);
        PQfinish(conn);
        return false;
    }

    qDebug() << "Connection opened. - " << PQerrorMessage(conn);
    return true;
}

bool PostgresDataProvider::executeQuery(const std::string query)
{
    qDebug() << "PostgresDataProvider::create";

    PGresult* res;
    static auto cleanFinish = [](PGconn* conn, PGresult* res){
        PQclear(res);
        PQfinish(conn);
    };

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        qDebug() << "BEGIN command failed! "<< PQerrorMessage(conn);
        cleanFinish(conn, res);
        return -1;
    }

    PQclear(res);

    res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        qDebug() << "executeQuery command failed! "<< PQerrorMessage(conn);
        cleanFinish(conn, res);
        return false;
    }

    res = PQexec(conn, "COMMIT");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        qDebug() << "COMMIT command failed! "<< PQerrorMessage(conn);
        cleanFinish(conn, res);
        return false;
    }

    cleanFinish(conn, res);

    return true;
}

int PostgresDataProvider::read(const std::string query)
{
    qDebug() << "PostgresDataProvider::executeQuery";

    PGresult* res;
    int nFields;
    int i;

    static auto cleanFinish = [](PGconn* conn, PGresult* res){
        PQclear(res);
        PQfinish(conn);
    };

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        qDebug() << "BEGIN command failed! "<< PQerrorMessage(conn);
        cleanFinish(conn, res);
        return -1;
    }

    PQclear(res);

    std::string curs = "DECLARE mcursor CURSOR FOR "+query;
    const char* sql = curs.c_str();
    res = PQexec(conn, sql);

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        qDebug() << "DECLARE CURSOR failed! " << PQerrorMessage(conn);
        cleanFinish(conn, res);
        return -1;
    }

    PQclear(res);

    res = PQexec(conn, "FETCH ALL in mcursor");
    if (PQresultStatus(res) != PGRES_TUPLES_OK){
        qDebug() << "FETCH ALL failed! " << PQerrorMessage(conn);
        cleanFinish(conn, res);
        return -1;
    }

    nFields = PQnfields(res);

    for(i=0; i<PQntuples(res); ++i){

        StringMapped* record = new StringMapped;

        for(int j=0; j<nFields; ++j){
            record->insert(std::make_pair(PQfname(res, j),
                          PQgetvalue(res, i, j)));
        }

        // cache results
        append(record);

        //vm.push_back(record);
    }

    PQclear(res);
    res = PQexec(conn, "CLEAR mcursor");
    PQclear(res);

    res = PQexec(conn, "END");
    PQclear(res);

   // PQfinish(conn);

    return cacheSize();
}
