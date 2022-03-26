#ifndef RAVENEXCEPTION_H
#define RAVENEXCEPTION_H

#include <string>


class RavenException
{
public:
    RavenException();
    virtual ~RavenException();
    virtual std::string errorMessage()=0;
    virtual std::string which()=0;
    virtual std::string action()=0;
    virtual void setAction(const std::string act)=0;
};

class DatabaseException : public RavenException
{
public:
    DatabaseException();
    ~DatabaseException() override;
    std::string errorMessage() override;
    std::string which() override;
    std::string action() override;
    void setAction(const std::string act) override;
};

class PostgresException : public DatabaseException
{
public:
    PostgresException(const std::string action, const std::string errorMsg);
    ~PostgresException()override;
    std::string errorMessage() override;
    std::string which() override;
    std::string action() override;
    void setAction(const std::string act) override;
private:
    std::string mErrorMsg;
    std::string mAction;
};

class AudioImportException : public RavenException
{
public:
    AudioImportException(const std::string action, const std::string error_msg);
    ~AudioImportException() override;
    std::string errorMessage() override;
    std::string which() override;
    std::string action() override;
    void setAction(const std::string act) override;
private:
    std::string mErrorMsg;
    std::string mAction;
};

#endif // RAVENEXCEPTION_H
