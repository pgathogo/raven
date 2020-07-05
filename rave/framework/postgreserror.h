#ifndef POSTGRESERROR_H
#define POSTGRESERROR_H

#include <string>

class PostgresError
{
public:
    PostgresError();
    PostgresError(const char* msg);
    std::string errorMessage();
private:
    std::string mErrorMsg;
};

#endif // POSTGRESERROR_H
