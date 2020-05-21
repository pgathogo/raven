#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>
#include <libpq-fe.h>

class DatabaseConnector
{
public:
    DatabaseConnector();
    virtual ~DatabaseConnector();
    virtual bool openConnection(const std::string conninfo)=0;
};

class PostgresConnector : public DatabaseConnector
{
    public:
        static PostgresConnector* instance(const std::string conninfo);
        ~PostgresConnector() override;
        bool openConnection(const std::string conninfo) override;
        PGconn* connection();
    protected:
        PostgresConnector(const std::string conninfo);
    private:
        std::string mConnInfo;
        PGconn* mConnection;
        static PostgresConnector* mInstance;
};

#endif // DATABASECONNECTOR_H
