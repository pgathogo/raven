#include <algorithm>
#include <QTableWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include "../../../rave/framework/entitydatamodel.h"
#include "ravensetup.h"

#include "commloganalyzer.h"
#include "ui_commloganalyzer.h"


CommLogAnalyzer::CommLogAnalyzer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CommLogAnalyzer)
{
    ui->setupUi(this);

    ui->twLogs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twLogs->setAlternatingRowColors(true);
    ui->twLogs->horizontalHeader()->setStretchLastSection(true);

    ui->pbReader->setValue(0);
    ui->pbReader->setTextVisible(true);
    ui->pbReader->setVisible(false);

    ui->dtLogDate->setDate(QDate::currentDate());

    setMinimumSize(900, 500);
    setWindowTitle("Commercial Log Analyzer");

    m_edm_setup = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
    m_edm_setup->all();
    if(m_edm_setup->count() > 0){
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());
    }

    // connect(ui->btnLoad, &QPushButton::clicked, this, &CommLogAnalyzer::on_load_log_file);
    connect(ui->dtLogDate, &QDateEdit::dateChanged, this, &CommLogAnalyzer::on_load_log_file);
}

int CommLogAnalyzer::count_lines(QFile& file)
{
    int count = 0;

    QTextStream in(&file);
    while (!in.atEnd()){
        in.readLine();
        count++;
    }

    file.seek(0);
    return count;
}

std::tuple<std::string, std::string, std::string> CommLogAnalyzer::parse_timestamp(const std::string& timestamp)
{
    // Find the separator between date and time
    size_t sep_pos = timestamp.find("__");

    if (sep_pos == std::string::npos) {
        return std::make_tuple("","","");
    }

    // Extract date port (before "__")
    std::string date_part = timestamp.substr(0, sep_pos);

    // Extract time part (after "__") and remove milliseconds
    std::string time_part = timestamp.substr(sep_pos + 3);
    size_t dot_pos = timestamp.find('.');

    if (dot_pos != std::string::npos) {
        time_part = time_part.substr(0, dot_pos);
    }

    // Parse HH:MM:SS format
    size_t first_colon = time_part.find(':');
    size_t second_colon = time_part.find(':', first_colon + 1);

    if (first_colon == std::string::npos || second_colon == std::string::npos) {
        return std::make_tuple("", "", "");
    }

    // Extract components
    std::string hour_str = time_part.substr(0, first_colon);
    std::string minute_str = time_part.substr(first_colon + 1, second_colon - first_colon - 1);
    std::string second_str = time_part.substr(second_colon + 1);

    // int hour = std::std::stoi(hour_str);

    std::string minute_second = minute_str + ":"+ second_str;

    return std::make_tuple(date_part, hour_str, minute_second);

}

void CommLogAnalyzer::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;
}

CommLogAnalyzer::~CommLogAnalyzer()
{
    delete ui;
}

QString CommLogAnalyzer::find_log_file_by_date(const QString& logdir, const QDate& log_date)
{
    QString datestr = log_date.toString("yyyy_MM_dd");
    QString filename = QString("magicsoft_playout_logger_%1.log").arg(datestr);

    // Create full path
    QDir dir(logdir);
    QString fullpath = dir.filePath((filename));

    // Check if file exists
    QFileInfo file_info(fullpath);
    if (file_info.exists() && file_info.isFile()) {
       return fullpath;
    }

    return QString();

}

void CommLogAnalyzer::on_load_log_file(const QDate& date)
{
    // TODO:: Check if logdir is empty
    QString logdir = m_setup->magicsoft_logfile_path()->to_qstring();

    QString logfile = find_log_file_by_date(logdir, date);

    if (logfile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No log file found for this date");
        return;
    }

    ui->edtLogFile->setText(logfile);

    //QString fname = "D:/Home/Clients/Baryonic/Stations/TV/logs/magicsoft_playout_logger_2025_09_01.log";
    parse_and_display(logfile);
    return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open Log file", "",
                                                    "Log Files (*.log);;All Files (*)");
    if (!filename.isEmpty()) {
        parse_and_display(filename);
    }
}


void CommLogAnalyzer::parse_and_display(const QString& filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open file");
        return;
    }

    ui->dtLogDate->setEnabled(false);
    ui->pbReader->setVisible(true);
    ui->pbReader->setFormat("Calculating lines...");
    QCoreApplication::processEvents();

    int total_lines = count_lines(file);

    ui->pbReader->setRange(0, total_lines);
    ui->pbReader->setValue(0);
    ui->pbReader->setFormat("%p% processed");

    // Clear previous data
    ui->twLogs->clear();
    ui->twLogs->setRowCount(0);

    QStringList headers;
    headers << "ID" << "PlayDate" << "PlayHour" << "PlayTime" << "-" << "CH" << "Server"
            << "Version" << "PID" << "User" << "Role" << "Type"
            << "Event" << "Details" << "Play ID" ;

    ui->twLogs->setColumnCount(headers.size());
    ui->twLogs->setHorizontalHeaderLabels(headers);

    ui->twLogs->setSortingEnabled(false);
    ui->twLogs->setUpdatesEnabled(false);

    int row = 0;
    int file_line_index = 0;
    int col_index_asrun = 9;      // "AsRun" appears in the 10th column (index 9)
    std::vector<int> timestamp_columns = {1, 2, 3};

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        file_line_index++;

        if (file_line_index % 100 == 0) {
            ui->pbReader->setValue(file_line_index);
            QCoreApplication::processEvents();
        }

        // Skip empty lines
        if (line.trimmed().isEmpty()) continue;

        // Split by Tab
        // KeepEmptyParts is important to maintain column alignment

        QStringList parts = line.split('\t', Qt::KeepEmptyParts);

        // Ensure line has enought columsn to be valid
        if (parts.size() <= col_index_asrun) continue;

        // Filter Logic:
        // We only want lines where column 9 is "AsRun"

        if (parts[col_index_asrun-1] == "def_cat")  continue;


        if (parts[col_index_asrun] == "AsRun")
        {
            ui->twLogs->insertRow(row);

            // --- COLUMN 0: ID ---
            ui->twLogs->setItem(row, 0, new QTableWidgetItem(parts[0]));

            // --- COLUMN 1 (Raw): Timestamp processing ---
            // Format: 2025-09-01__00:04:06.570
            QString raw_timestamp = parts[1];
            QString date_part = "";
            QString hour_part_str = "";
            int hour_part = 0;
            QString min_sec_part = "";

            // Split "2025-09-01" from "00:04:06.570"
            QStringList date_and_time = raw_timestamp.split("___");

            if (date_and_time.size() > 0) {
                date_part = date_and_time[0];  // "2025-09-01"
            }

            if (date_and_time.size() > 1) {
                // Process "00:04:06.570"
                QString time_only = date_and_time[1];
                QStringList time_pieces = time_only.split(':');

                if (time_pieces.size() >= 3) {
                    // Hour
                    hour_part_str = time_pieces[0];
                    hour_part = time_pieces[0].toInt(); // Convert "00" to integer 0

                    QString minute = time_pieces[1];
                    QString second = time_pieces[2].split('.')[0]; // Split by dot to remove milliseconds

                    min_sec_part = minute + ":" + second;
                }
            }

            // --- INSERT NEW COLUMNS (Table Cols 1, 2, 3) ---

            // Date
            ui->twLogs->setItem(row, 1, new QTableWidgetItem(date_part));

            // Hour (We use QTableWidgetItem with specific data role for correct numeric sorting

            QTableWidgetItem* hour_item = new QTableWidgetItem(hour_part_str);
            //hour_item->setData(Qt::DisplayRole, hour_part);
            ui->twLogs->setItem(row, 2, hour_item);

            // Min:Sec
            ui->twLogs->setItem(row, 3, new QTableWidgetItem(min_sec_part));

            // --- REMAINING COLUMNS ---
            // Raw columns 2 to End match Table columns 4 to End

            int target_col = 4;
            for(int src_col = 2; src_col < parts.size(); ++src_col) {
                // Expand table if raw file has extra columns we didn't expect

                if(target_col >= ui->twLogs->columnCount()) {
                    ui->twLogs->insertColumn(ui->twLogs->columnCount());
                }

                QTableWidgetItem* item = new QTableWidgetItem(parts[src_col]);
                ui->twLogs->setItem(row, target_col, item);
                target_col++;
            }

            row++;

        } // AsRun

    } // While

    ui->pbReader->setValue(total_lines);
    ui->twLogs->setUpdatesEnabled(true);
    ui->twLogs->setSortingEnabled(true);
    ui->dtLogDate->setEnabled(true);

    file.close();

}
