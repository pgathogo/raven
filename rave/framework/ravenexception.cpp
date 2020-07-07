#include "ravenexception.h"

RavenException::RavenException()
{

}
RavenException::~RavenException()
{
};

std::string RavenException::errorMessage()
{
    return "raven exceptions";
}
std::string RavenException::which()
{
    return "RavenException";
}

// ------ DatabaseException -----
DatabaseException::DatabaseException()
{
}

DatabaseException::~DatabaseException()
{
}

// ------ PostgresException -----
PostgresException::PostgresException(const char* errorMsg)
{
    mErrorMsg = errorMsg;
}

PostgresException::~PostgresException()
{
}

std::string PostgresException::errorMessage()
{
    return mErrorMsg;
}

std::string PostgresException::which()
{
    return "PostgresException";
}
