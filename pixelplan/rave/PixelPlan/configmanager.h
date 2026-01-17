#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <tuple>

#include <QJsonDocument>

namespace PIXELPLAN
{
    using ErrorOr = std::tuple<bool, QString>;

    class ConfigManager {
    public:
        explicit ConfigManager();
        ErrorOr read_config(const QString&);
        ErrorOr write_config();
        ErrorOr update_value(const QString&, const QString&);
        QString get_value(const QString&);
    private:
        QJsonDocument m_json_doc;
        QString m_config_filepath;
    };

}



#endif
