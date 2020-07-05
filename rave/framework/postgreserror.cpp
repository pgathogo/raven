#include "postgreserror.h"

PostgresError::PostgresError()
    :mErrorMsg{nullptr}
{
}

PostgresError::PostgresError(const char* msg)
{
    mErrorMsg = msg;
}
std::string PostgresError::errorMessage()
{
    return mErrorMsg;
}
