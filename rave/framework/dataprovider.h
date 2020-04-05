#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include <string>
#include <vector>
#include <map>
#include <libpq-fe.h>
#include "queryset.h"

//using VectorMapped = std::vector<std::map<std::string, std::string>>;
using StringMapped = std::map<std::string, std::string>;
//using StringTuple = std::tuple<std::string, std::string>;

class BaseDataProvider
{
public:
    BaseDataProvider();
    virtual ~BaseDataProvider();
    virtual int executeQuery(const std::string query){ return -1; };
    void append(StringMapped* data);
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
        ~PostgresDataProvider() override;
        int executeQuery(const std::string query) override;
        void test() {}
    private:
        PGconn* conn;
        bool openConnection();
};

#endif // DATAPROVIDER_H
