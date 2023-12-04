#ifndef CACHEHANDLER_H
#define CACHEHANDLER_H

#include <QJsonObject>

class JsonCacheHandler
{
public:
    JsonCacheHandler(QString, QJsonObject);
    void read_cache();
    void write_cache(QJsonObject);
    QJsonObject get_cache();
private:
    QString m_filename;
    QJsonObject m_data;
};

#endif
