#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <string>
#include <vector>
#include <QDebug>
#include <map>

#include <libpq-fe.h>
#include <sqlite3.h>

#include "queryset.h"

//using VectorMapped = std::vector<std::map<std::string, std::string>>;
using StringMapped = std::map<std::string, std::string>;
using RecordTuple = std::tuple<std::string, std::string>;
using VectorMapped = std::vector<std::map<std::string, std::string>>;

class PostgresConnector;
class DatabaseException;

class BaseDataProvider
{
public:
    BaseDataProvider();
    virtual ~BaseDataProvider();
    virtual bool executeQuery(const std::string query)=0;
    virtual int read(const std::string query) = 0;
    virtual int fetchLastId(const std::string tableName)=0;
    virtual int insert_returning_id(const std::string query)=0;
    void append(StringMapped* data);
    void clear();
    int cacheSize() const;
    std::vector<StringMapped*>::iterator cacheIter();
    DataQuerySet<StringMapped>* cache();
    virtual std::string provider_type() = 0;
private:
    DataQuerySet<StringMapped> dataQuerySet;
};


class PostgresDataProvider: public BaseDataProvider
{
    public:
        PostgresDataProvider();
        //static PostgresDataProvider* instance();
        ~PostgresDataProvider() override;
        bool executeQuery(const std::string query) override;
        int fetchLastId(const std::string tableName) override;
        int insert_returning_id(const std::string query) override;
        int read(const std::string query) override;
        void test() { qDebug() << "XXXX->" << "PostgresDataProvider"; }
        void openConnection();
        void openConnection(const std::string conninfo);
        void closeConnection();
        char* make_error_message(char*, char*);
        std::string provider_type() override;
        void nullify_connector();
        static void test_connection(const std::string conninfo);
    private:
        PostgresConnector* mPGConnector;
        PGconn* conn;
        //static PostgresDataProvider* mInstance;
};

struct SQLiteResult
{
public:
    SQLiteResult(){}
    int result_status{ 0 };
    const char* result_message;
};


class SQLiteDataProvider : public BaseDataProvider
{
    public:
        SQLiteDataProvider(const std::string);
        //static PostgresDataProvider* instance();
        ~SQLiteDataProvider() override;
        bool executeQuery(const std::string query) override;
        int fetchLastId(const std::string tableName) override;
        int insert_returning_id(const std::string query) override;
        int read(const std::string query) override;
        SQLiteResult open_connection();
        char* make_error_message(char*, char*);

        void add_record(StringMapped*);
        int fetch_data(const std::string);
        bool exec_query(const std::string);

        const std::string db_name();
        std::string provider_type() override;

        static int read_data(void*, int, char**, char**);
        static int exec_return_id(void* , int , char** , char** );
        static int empty_callback(void*, int, char**, char**);

    private:
        std::string m_dbname;
        sqlite3* m_conn;

};



#endif // DATAPROVIDER_H
