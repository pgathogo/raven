#include <map>
#include <iostream>
#include <QDebug>
#include "dataprovider.h"
#include "queryset.h"
#include "databaseconnector.h"
#include "ravenexception.h"
#include "Logger.h"

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
    //,mPGConnector{nullptr}
{
}

PostgresDataProvider::~PostgresDataProvider()
{
    qDebug() << "PostgresDataProvider::dtor";
    //PQfinish(conn);
}

void PostgresDataProvider::test_connection(const std::string conninfo)
{
    try{
         PostgresConnector::test_connection(conninfo);
    }catch (DatabaseException& de){
        throw;
    }

    /*
    try{
        return true;
    }catch(DatabaseException& de){
        return false;
    }
   */
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
}


void PostgresDataProvider::openConnection(const std::string conninfo)
{
    mPGConnector = PostgresConnector::instance(conninfo);
    conn = mPGConnector->connection();
}

/*
void PostgresDataProvider::test_connection(const std::string conninfo)
{
    mPGConnector = PostgresConnector::instance(conninfo);
    conn = mPGConnector->connection();
}
*/

void PostgresDataProvider::closeConnection()
{
    PQfinish(conn);
}

void PostgresDataProvider::nullify_connector()
{
    mPGConnector->nullify_instance();
}

bool PostgresDataProvider::executeQuery(const std::string query)
{

    PGresult* res = nullptr;

    static auto cleanFinish = [](PGresult* res){
        PQclear(res);
    };

    if (res != nullptr)
        PQclear(res);

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        std::string errorMsg = "BEGIN command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("EXCUTE-BEGIN", errorMsg);
    }

    PQclear(res);

    res = PQexec(conn, query.c_str());

    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        std::string errorMsg = "EXECUTE command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("EXCEUTE", errorMsg);
    }

    res = PQexec(conn, "COMMIT");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string errorMsg = "COMMIT command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("COMMIT", errorMsg);
    }

    cleanFinish(res);

    return true;
}

int PostgresDataProvider::insert_returning_id(const std::string query)
{

    PGresult* res = nullptr;

    static auto cleanFinish = [](PGresult* res){
        PQclear(res);
    };

    if (res != nullptr)
        PQclear(res);

    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK){
        std::string errorMsg = "BEGIN command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("EXCUTE-BEGIN", errorMsg);
    }

    PQclear(res);

    char buffer[30];
    char* p = buffer;

    res = PQexecParams(conn, query.c_str(), 0, nullptr, &p, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::string errorMsg = "EXECUTE command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("EXCEUTE", errorMsg);
    }

    std::string last_id(PQgetvalue(res, 0, 0));

    res = PQexec(conn, "COMMIT");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string errorMsg = "COMMIT command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(res);
        throw PostgresException("COMMIT", errorMsg);
    }

    cleanFinish(res);

    return std::stoi(last_id);

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
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string errorMsg = "BEGIN command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(conn, res);
        Logger::error("PostgresDataProvider", "Function `read`..."+QString::fromStdString(errorMsg));
        throw PostgresException("READ-BEGIN", errorMsg);
    }

    PQclear(res);

    std::string curs = "DECLARE mcursor CURSOR FOR "+query;
    const char* sql = curs.c_str();
    res = PQexec(conn, sql);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        std::string errorMsg = "DECLARE command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(conn, res);


        Logger::error("PostgresDataProvider", "Function `read`..."+QString::fromStdString(errorMsg));

        return 0;

        // throw PostgresException("READ-DECLARE-CURSOR", errorMsg);
    }

    PQclear(res);

    res = PQexec(conn, "FETCH ALL in mcursor");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        std::string errorMsg = "FETCH ALL command failed!\n";
        errorMsg += PQerrorMessage(conn);
        PQexec(conn, "ROLLBACK");
        cleanFinish(conn, res);
        throw PostgresException("READ-FETCH-ALL", errorMsg);
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

std::string PostgresDataProvider::provider_type()
{
    return ">> POSTGRES <<";
}

/* ----- SQLiteDataProvider --------------------- */

SQLiteDataProvider::SQLiteDataProvider(const std::string dbname)
    :m_dbname{ dbname }
    ,m_conn{ nullptr }
{
    qDebug() << "SQLiteDataProvider::ctor :"<< QString::fromStdString(m_dbname);
}

SQLiteDataProvider::~SQLiteDataProvider()
{
    if (m_conn != nullptr )
        delete m_conn;
}

int SQLiteDataProvider::read_data(void* context, int argc, char** argv, char** azColName)
{
    StringMapped* record = new StringMapped;
    for (int i=0; i<argc; ++i){
        record->insert(std::make_pair(azColName[i], (argv[i] ? argv[i] : "NULL")));
    }

    SQLiteDataProvider* db_provider = reinterpret_cast<SQLiteDataProvider*>(context);
    db_provider->add_record(record);

    return 0;

}

int SQLiteDataProvider::exec_return_id(void* _id, int argc, char** argv, char** azColName)
{
    const char* last_id = argv[0];
    return std::stoi(last_id);
}

int SQLiteDataProvider::empty_callback(void* _id, int argc, char** argv, char** azColName)
{
    return 0;
}

void SQLiteDataProvider::add_record(StringMapped* record)
{
    //m_data.push_back(record);
    append(record);
}

int SQLiteDataProvider::fetch_data(const std::string query)
{
}

bool SQLiteDataProvider::exec_query(const std::string query)
{
    SQLiteResult result = open_connection();
    const char* sql = query.c_str();
    char* err_msg = 0;

    result.result_status = sqlite3_exec(m_conn, sql, SQLiteDataProvider::read_data, this, &err_msg);

    if (result.result_status != SQLITE_OK){
        // Todo: log error messages
        //::cout << "SQL error: " << sqlite3_errmsg(m_database) << '\n';
        sqlite3_free(err_msg);
        return false;
    }

    sqlite3_close(m_conn);
    return true;
}

SQLiteResult SQLiteDataProvider::open_connection()
{
    qDebug() << "DBName: " << QString::fromStdString(m_dbname);

    SQLiteResult result;
    result.result_status = sqlite3_open(m_dbname.c_str(), &m_conn);
    result.result_message = sqlite3_errmsg(m_conn);
    return result;
}

char* SQLiteDataProvider::make_error_message(char*, char*)
{

}

bool SQLiteDataProvider::executeQuery(const std::string query)
{
    SQLiteResult result = open_connection();
    const char* sql = query.c_str();
    char* err_msg = 0;

    int last_id = -1;
    result.result_status = sqlite3_exec(m_conn, sql, SQLiteDataProvider::empty_callback, this, &err_msg);

    if (result.result_status != SQLITE_OK){
       sqlite3_free(err_msg);
       return false;
    }

    sqlite3_close(m_conn);
    return true;
}

int SQLiteDataProvider::fetchLastId(const std::string tableName)
{

}

int SQLiteDataProvider::read(const std::string query)
{
    SQLiteResult result = open_connection();

    const char* sql = query.c_str();
    char* err_msg = 0;

    result.result_status = sqlite3_exec(m_conn, sql, SQLiteDataProvider::read_data, this, &err_msg);

    if (result.result_status != SQLITE_OK){
        // raise exception
        sqlite3_free(err_msg);
    }

    sqlite3_close(m_conn);

    return cacheSize();
}

const std::string SQLiteDataProvider::db_name()
{
    return m_dbname;
}

std::string SQLiteDataProvider::provider_type()
{
    return ">> SQLITE <<";
}

int SQLiteDataProvider::insert_returning_id(const std::string query)
{
    SQLiteResult result = open_connection();

    const char* sql = query.c_str();
    char* err_msg = 0;

    int last_id = -1;
    result.result_status = sqlite3_exec(m_conn, sql, SQLiteDataProvider::exec_return_id, &last_id, &err_msg);

    if (result.result_status != SQLITE_OK){
       // raise exception
       sqlite3_free(err_msg);
       return -1;
    }

    last_id = sqlite3_last_insert_rowid(m_conn);

    sqlite3_close(m_conn);

    return last_id;
}
