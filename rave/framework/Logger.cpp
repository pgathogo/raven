#include "Logger.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QObject>

QFile* Logger::logFile = Q_NULLPTR;
bool Logger::isInit = false;

QHash<QtMsgType, QString> Logger::contextNames = {
    {QtMsgType::QtDebugMsg,		"ERROR"},
    {QtMsgType::QtInfoMsg,		"INFO "},
    {QtMsgType::QtWarningMsg,	"WARNING"},
    {QtMsgType::QtCriticalMsg,	"CRITICAL"},
    {QtMsgType::QtFatalMsg,		"ERROR"}
};

std::vector<QString> dow{"None", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

bool Logger::add_file_header(QString log_filepath)
{
    QFile logfile(log_filepath);
    if(!logfile.open(QIODevice::Append | QIODevice::Text))
        return false;

    QString time_stamp =  QDateTime::currentDateTime().toString("dd/MM/yyyy HH:MM");

    QTextStream out(&logfile);
    QDate d = QDateTime::currentDateTime().date();
    out << "---------------------------------------------------------------------------------" << "\n";
    out << dow[d.dayOfWeek()]<<"  "<< time_stamp << "\n";
    out << "---------------------------------------------------------------------------------" << "\n";

    logfile.close();

    return true;
}

bool Logger::add_file_timestamp(QString log_filepath)
{
    QFile logfile(log_filepath);
    if(!logfile.open(QIODevice::Append | QIODevice::Text))
        return false;

    QString time_stamp =  QDateTime::currentDateTime().toString("dd/MM/yyyy HH:MM");

    QTextStream out(&logfile);
    out << "\n";
    out << "---------------------------------------------------------------------------------" << "\n";
    out << time_stamp << "\n";
    out << "---------------------------------------------------------------------------------" << "\n";

    logfile.close();

    return true;
}

void Logger::init(QString log_file) {
	if (isInit) {
		return;
	}

    logFile = new QFile;
    logFile->setFileName(log_file);

    if (QFile::exists(log_file)){
        bool result = Logger::add_file_timestamp(log_file);
        if (!result)
            return;
    } else {
        bool result = Logger::add_file_header(log_file);
        if (!result)
            return;
    }

    logFile->open(QIODevice::Append | QIODevice::Text);

	// Redirect logs to messageOutput
	qInstallMessageHandler(Logger::messageOutput);

	// Clear file contents
    //logFile->resize(0);

	Logger::isInit = true;
}

void Logger::clean() {
	if (logFile != Q_NULLPTR) {
		logFile->close();
		delete logFile;
	}
}

void Logger::info(const QString module, QString msg)
{
    qInfo().noquote()
        << QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")
        << module
        << ";"
        << msg;
}

void Logger::debug(const QString module, QString msg)
{
    qDebug().noquote() << module << ";" << msg;
}

void Logger::error(const QString module, QString msg)
{
    qDebug().noquote() << module << ";" << msg;
}


void Logger::messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString module = "";
    QString msg_body = msg;

    QStringList msg_list = msg.split(';');

    if (msg_list.length() > 1){
        module = msg_list[0];
        msg_body = msg_list[1];
    }

    QString log = QObject::tr("%1: %2 - %3- %4\n").
        arg(Logger::contextNames.value(type)).
        arg(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss")).
        arg(module).
        arg(msg_body);

                  /*
        arg(context.line).
        arg(QString(context.file).
            section('\\', -1)).			// File name without file path
        arg(QString(context.function).
			section('(', -2, -2).		// Function name only
			section(' ', -1).
			section(':', -1)).
        arg(msg_body);
            */

	logFile->write(log.toLocal8Bit());
	logFile->flush();
}
