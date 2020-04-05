#include <string>
#include <QString>

QString strtoqstr(std::string s)
{
    return QString::fromStdString(s);
}
