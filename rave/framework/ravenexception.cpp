#include "ravenexception.h"

RavenException::RavenException()
{

}
RavenException::~RavenException()
{
};

// ------ DatabaseException -----
DatabaseException::DatabaseException()
{
}

DatabaseException::~DatabaseException()
{
}


std::string DatabaseException::errorMessage(){}
std::string DatabaseException::which(){}
std::string DatabaseException::action(){}
void DatabaseException::setAction(const std::string act){}

// ------ PostgresException -----
PostgresException::PostgresException(const std::string action, const std::string errorMsg)
{
    mAction = action;
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

std::string PostgresException::action()
{
    return mAction;
}

void PostgresException::setAction(const std::string act)
{
    mAction = act;
}

// ------ AudioImportException -----

AudioImportException::AudioImportException(const std::string action, const std::string error_msg)
    :mAction{action}
    ,mErrorMsg{error_msg}
{
}

AudioImportException::~AudioImportException()
{
}

std::string AudioImportException::errorMessage()
{
    return mErrorMsg;
}

std::string AudioImportException::which()
{
    return "AudioImportException";
}

std::string AudioImportException::action()
{
}

void AudioImportException::setAction(const std::string act)
{
    mAction = act;
}
