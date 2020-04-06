#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include <QString>

namespace raven{
    QString strtoq(std::string s)
    {
        return QString::fromStdString(s);
    }
};

#endif
