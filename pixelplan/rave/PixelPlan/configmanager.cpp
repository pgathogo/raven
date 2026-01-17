#include <QFileInfo>
#include <QFile>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonParseError>

#include "configmanager.h"


namespace PIXELPLAN
{

  ConfigManager::ConfigManager()
  {
  }

  ErrorOr ConfigManager::read_config(const QString& config_filepath)
  {
      // Check if config file exists
      if (config_filepath.isEmpty())
          return std::make_tuple(false, "Empty config file");

      QFileInfo fi(config_filepath);
      if (!fi.exists())
          return std::make_tuple(false, "Config file does not exists!");


      QFile file(config_filepath);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          QString msg = QString("Could not open file: %1").arg(config_filepath);
          return std::make_tuple(false, msg);
      }

      m_config_filepath = config_filepath;

      QByteArray json_data = file.readAll();

      file.close();

      QJsonParseError parser_error;
      m_json_doc = QJsonDocument::fromJson(json_data, &parser_error);

      if (parser_error.error != QJsonParseError::NoError) {
          QString msg = QString("JSON parse error:").arg(parser_error.errorString());
          return std::make_tuple(false, msg);
      }

      return std::make_tuple(true, "Config file loaed successfully");

  }

ErrorOr ConfigManager::write_config()
{
    if (m_config_filepath.isEmpty()) {
        return std::make_tuple(false, "No config file!");
    }

    QFile file(m_config_filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        QString msg = QString("Could not open file for writting: %1").arg(m_config_filepath);
        return std::make_tuple(false, msg) ;
    }

    QByteArray json_data = m_json_doc.toJson(QJsonDocument::Indented);
    file.write(json_data);
    file.close();

    return std::make_tuple(true, "File saved successfully");

}

ErrorOr ConfigManager::update_value(const QString& key, const QString& value)
{
    if(!m_json_doc.isObject()) {
        return std::make_tuple(false, "Invalid JSON document");
    }

    QJsonObject root_object = m_json_doc.object();

    if (!root_object.contains("setup") || !root_object["setup"].isObject()) {
        return std::make_tuple(false, "Missing `setup` object");
    }

    QJsonObject setup_object = root_object["setup"].toObject();
    setup_object[key] = value;

    root_object["setup"] = setup_object;
    m_json_doc.setObject(root_object);

    return std::make_tuple(true, "Value saved successfully.");

}

QString ConfigManager::get_value(const QString& key)
{
    if (!m_json_doc.isObject())
        return QString();

    QJsonObject root_object = m_json_doc.object();

    if (!root_object.contains("setup") || !root_object["setup"].isObject()) {
        return QString();
    }

    QJsonObject setup_object = root_object["setup"].toObject();

    if (!setup_object.contains(key)) {
        return QString();
    }

    return setup_object[key].toString();
}




}
