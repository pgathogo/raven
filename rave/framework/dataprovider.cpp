#include <QDebug>
#include <map>
#include "dataprovider.h"
#include "queryset.h"
#include "databaseconnector.h"
#include "postgreserror.h"

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

void BaseDataProvider::clear()
{
    dataQuerySet.clear();
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

//PostgresDataProvider* PostgresDataProvider::mInstance = nullptr;

PostgresDataProvider::PostgresDataProvider()
    : BaseDataProvider()
    ,mPGConnector{nullptr}
{
    qDebug() << "Open connection ...";
    openConnection();
}

PostgresDataProvider::~PostgresDataProvider()
{
    //PQfinish(conn);
}

/*
PostgresDataProvider* PostgresDataProvider::instance()
{
    if (mInstance == nullptr){
        mInstance = new PostgresDataProvider;
    }

    return mInstance;
}
*/

void PostgresDataProvider::openConnection()
{
    // read this values from registry
    //const char* conninfo = "user=postgres password=abc123 dbname=raven port=5433";
    std::string conninfo = "user=postgres password=abc123 dbname=raven port=5433";

    mPGConnector = PostgresConnector::instance(conninfo);

    conn = mPGConnector->connection();


    /*
    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK){
        qDebug() << "Connection to database failed! - " << PQerrorMessage(conn);
        PQfinish(conn);
        return false;
    }
    qDebug() << "Connection opened. - " << PQerrorMessage(conn);
    return true;
    */

}

bool PostgresDataProvider::executeQuery(const std::string query)
{
    PGresult* res = nullptr;

    static auto cleanFinish = [](PGconn* conn, PGresult* res){
        PQclear(res);
    };

    if (res != nullptr)
        PQclear(res);

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        char* cmdError = "BEGIN command failed!\n";
        char* errorMsg = make_error_message(cmdError, PQerrorMessage(conn));
        cleanFinish(conn, res);
        throw PostgresError(errorMsg);
    }

    PQclear(res);

    res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        char* cmdError = "EXECUTE command failed!\n";
        char* errorMsg = make_error_message(cmdError, PQerrorMessage(conn));
        cleanFinish(conn, res);
        throw PostgresError(errorMsg);
    }

    res = PQexec(conn, "COMMIT");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        char* cmdError = "COMMIT command failed!\n";
        char* errorMsg = make_error_message(cmdError, PQerrorMessage(conn));
        cleanFinish(conn, res);
        throw PostgresError(errorMsg);
    }

    cleanFinish(conn, res);

    return true;
}

int PostgresDataProvider::read(const std::string query)
{

    clear();

    PGresult* res;
    int nFields;
    int i;

    static auto cleanFinish = [](PGconn* conn, PGresult* res){
        PQclear(res);
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

int PostgresDataProvider::fetchLastId(const std::string tableName)
{
    std::string query = "SELECT max(id) AS id from "+tableName;

    clear();

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

    std::string lastId;

    for(i=0; i<PQntuples(res); ++i){
        //RecordTuple record;
        for(int j=0; j<nFields; ++j){
            //record = std::make_tuple(PQfname(res, j), PQgetvalue(res, i, j));
            lastId = PQgetvalue(res, i, j);
        }
    }

    if (lastId.empty())
        lastId = "-1";

    PQclear(res);
    res = PQexec(conn, "CLEAR mcursor");
    PQclear(res);

    res = PQexec(conn, "END");
    PQclear(res);

    return std::stoi(lastId);
}

char* PostgresDataProvider::make_error_message(char* cmdError, char* pgError)
{
    char* msg = new char[strlen(cmdError)+strlen(pgError)+1];
    strcpy(msg, cmdError);
    strcat(msg, pgError);
    return msg;
}
