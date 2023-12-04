#include <QJsonDocument>
#include <QFile>
#include "cachehandler.h"

JsonCacheHandler::JsonCacheHandler(QString file_name, QJsonObject data)
    :m_filename{file_name}
    ,m_data{data}
{
    if (!QFile::exists(m_filename))
        write_cache(data);
}

void JsonCacheHandler::read_cache()
{
    QFile file(m_filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString content = file.readAll();
    file.close();

    QJsonDocument sd = QJsonDocument::fromJson(content.toUtf8());
    m_data = sd.object();
}

QJsonObject JsonCacheHandler::get_cache()
{
    read_cache();
    return m_data;
}

void JsonCacheHandler::write_cache(QJsonObject data)
{
    QFile file(m_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QJsonDocument jdoc(data);
    file.write(jdoc.toJson(QJsonDocument::Indented));
}
