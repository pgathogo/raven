#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <string>
#include <vector>
#include <map>
#include <libpq-fe.h>
#include "queryset.h"

//using VectorMapped = std::vector<std::map<std::string, std::string>>;
using StringMapped = std::map<std::string, std::string>;
using RecordTuple = std::tuple<std::string, std::string>;

class PostgresConnector;

class BaseDataProvider
{
public:
    BaseDataProvider();
    virtual ~BaseDataProvider();
    virtual bool executeQuery(const std::string query) = 0;
    virtual int read(const std::string query) = 0;
    virtual int fetchLastId(const std::string tableName)=0;
    void append(StringMapped* data);
    void clear();
    int cacheSize() const;
    std::vector<StringMapped*>::iterator cacheIter();
    DataQuerySet<StringMapped>* cache();
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
        int read(const std::string query) override;
        void test() {}
        void openConnection();
    private:
        PostgresConnector* mPGConnector;
        PGconn* conn;
        //static PostgresDataProvider* mInstance;
};

#endif // DATAPROVIDER_H
