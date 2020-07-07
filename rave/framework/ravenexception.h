#ifndef RAVENEXCEPTION_H
#define RAVENEXCEPTION_H

#include <string>


class RavenException
{
public:
    RavenException();
    virtual ~RavenException();
    virtual std::string errorMessage();
    virtual std::string which();
};

class DatabaseException : public RavenException{
public:
    DatabaseException();
    ~DatabaseException() override;
};

class PostgresException : public DatabaseException{
public:
    PostgresException(const char* errorMsg);
    ~PostgresException()override;
    std::string errorMessage() override;
    virtual std::string which() override;
private:
    std::string mErrorMsg;
};

#endif // RAVENEXCEPTION_H
