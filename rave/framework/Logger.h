#pragma once

#include <QDebug>
#include <QFile>
#include <QHash>
#include <QString>

class Logger {
private:
	/// @brief The file object where logs are written to.
	static QFile* logFile;

	/// @brief Whether the logger has being initialized.
	static bool isInit;

	/// @brief The different type of contexts.
	static QHash<QtMsgType, QString> contextNames;

public:
    static bool add_file_header(QString);
    static bool add_file_timestamp(QString);

    /// @brief Initializes the logger.
    static void init(QString);

	/// @brief Cleans up the logger.
	static void clean();

	/// @brief The function which handles the logging of text.
	static void messageOutput(QtMsgType type, const QMessageLogContext& context,
		const QString& msg);
};
