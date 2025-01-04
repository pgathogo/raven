#include <sstream>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QTextStream>

#include <iostream>
#include <fstream>

#include "commlogform.h"
#include "ui_commlogform.h"
#include "../utils/qchecklist.h"
#include "../framework/entitydatamodel.h"

CommLogForm::CommLogForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommLogForm)
{
    ui->setupUi(this);
    //set_hour_combobox();

//    ui->dtCommDate->setDate(QDate::currentDate());
//    connect(ui->dtCommDate, &QDateEdit::dateChanged, this, &CommLogForm::comm_log_date_changed);

    connect(ui->btnDateTime, &QPushButton::clicked, this, &CommLogForm::on_clicked_datetime);

//    connect(this->m_hours, QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, &CommLogForm::comm_log_hours_changed);
//    connect(this->m_hours, &QCheckList::signalPopupHidden, this, &CommLogForm::hours_popup_hidden);

    connect(ui->cbExpand, &QCheckBox::stateChanged, this, &CommLogForm::change_view_mode);
    connect(ui->cbAllHours, &QCheckBox::stateChanged, this, &CommLogForm::select_all_hours);
    connect(ui->btnPrintSum, &QPushButton::clicked, this, &CommLogForm::print_log);


//    setWindowState(this->windowState() | Qt::WindowFullScreen);

    m_process = std::make_unique<QProcess>(this);
    connect(m_process.get(), &QProcess::started, this, &CommLogForm::proc_started);
    connect(m_process.get(), &QProcess::errorOccurred, this, &CommLogForm::error_occured);

    set_default_dts();

    ui->btnDateTime->setText("Date: "+m_dts.sel_date.toString());

    std::string hours = comma_sep(m_dts.sel_hours);
    update_hours_label(hours);

    fetch_bookings(m_dts);

    this->setMinimumSize(QSize(950, 500));

    setWindowTitle("Commercial Logs");
}

CommLogForm::~CommLogForm()
{
    delete ui;
}

void CommLogForm::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;
}

void CommLogForm::set_default_dts()
{
    int prev_hour = 0;
    int next_hour = 0;
    int curr_hour = QTime::currentTime().hour();

    if ((curr_hour - 1) < 0){
        prev_hour = 0;
    }else {
        prev_hour = curr_hour - 1;
    }

    if ((curr_hour + 1) > 23){
        next_hour = 23;
    }else {
        next_hour = curr_hour + 1;
    }

    m_dts.sel_date = QDate::currentDate();

    m_dts.sel_hours.push_back(prev_hour);
    m_dts.sel_hours.push_back(curr_hour);
    m_dts.sel_hours.push_back(next_hour);

    m_dts.sel_date = QDate::currentDate();

}

void CommLogForm::on_clicked_datetime()
{
    //m_dts.clear();
    std::unique_ptr<DateTimeSelector> dts = std::make_unique<DateTimeSelector>(this, m_dts);

    if (dts->exec() == 1)
    {
        m_dts = dts->selection();

        std::string hours = comma_sep(m_dts.sel_hours);
        update_hours_label(hours);
        fetch_bookings(m_dts);

        ui->btnDateTime->setText("Date: "+m_dts.sel_date.toString());
    }

}

void CommLogForm::comm_log_hours_changed(int /*i*/)
{
    fetch_bookings(m_dts);
}

void CommLogForm::change_view_mode(int state)
{
    if (state == Qt::Checked)
        ui->tvCommLog->expandAll();
    else
        ui->tvCommLog->collapseAll();
}

void CommLogForm::select_all_hours(int state)
{
    for (std::size_t i=0; i < m_check_items.size(); ++i){
        if (state == 0){
            m_check_items[i]->setCheckState(Qt::Unchecked);
        } else {
            m_check_items[i]->setCheckState(Qt::Checked);
        }
    }

    hours_popup_hidden();

}

void CommLogForm::hours_popup_hidden()
{
    fetch_bookings(m_dts);
}

void CommLogForm::set_hour_combobox()
{
    m_hours = new QCheckList();

    for (int hr=0; hr < 24; ++hr){
        QString s_hr = QString::number(hr);
        if (s_hr.length() < 2)
            s_hr.prepend("0");
        m_check_items.push_back(m_hours->addCheckItem(s_hr, QVariant(hr), Qt::Unchecked));
        ui->hlHour->addWidget(m_hours);
    }

    ui->hlHour->setStretch(1,1);
    QSpacerItem* hspacer = new QSpacerItem(20, 20);
    ui->hlHour->addItem(hspacer);

}

std::string CommLogForm::comma_sep(const std::vector<int>& vals)
{
    std::string str="";
    int i=0;
    for(int v : vals){
        str += std::to_string(v);
        if (i < vals.size()-1)
            str += ",";
        ++i;
    }
    return str;
}

void CommLogForm::update_hours_label(std::string hours)
{
    hours = "["+hours+"]";
    ui->lblSelHours->setText("Hours: "+QString::fromStdString(hours));
}



void CommLogForm::fetch_bookings(const DateTimeSelection& dts)
{
    if (dts.sel_hours.size() == 0)
        return;

    std::string selected_hours = comma_sep(dts.sel_hours);
    selected_hours = "("+selected_hours+")";

    std::stringstream sql;

    sql << "select a.name AS client_name, b.id AS spot_id, b.name AS spot_name, "
        << "b.spot_duration AS spot_duration,  c.booking_status AS booking_status, "
        << "c.play_date AS play_date, c.play_time AS play_time, c.id AS booking_id, "
        << "e.schedule_date AS schedule_date, e.schedule_time AS schedule_time "
        << " from rave_client a, rave_spot b, rave_orderbooking c, rave_bookingsegment d, "
        << " rave_schedule e "
        << " where c.bookingsegment_id = d.id "
        << " and c.spot_id = b.id "
        << " and c.schedule_id = e.id "
        << " and b.client_id = a.id "
        << " and e.schedule_date = '"+dts.sel_date.toString("yyyy-MM-dd").toStdString()+"'"
        << " and e.schedule_hour in "+selected_hours
        << " order by e.schedule_date, e.schedule_time ";


    EntityDataModel edm;
    edm.readRaw(sql.str());

    std::cout << sql.str() << '\n';

    auto provider =  edm.getDBManager()->provider();

    m_comm_logs.clear();

    if (provider->cacheSize() > 0 ) {
        provider->cache()->first();
        do {
            auto itB = provider->cache()->currentElement()->begin();
            auto itE = provider->cache()->currentElement()->end();

            CommercialLog comm_log;

            for (; itB != itE; ++itB){
                std::string field =  (*itB).first;
                std::string value = (*itB).second;

                if (field == "client_name")
                    comm_log.client_name = value;
                if (field == "spot_id")
                    comm_log.spot_id = std::stoi(value);
                if (field == "spot_name")
                    comm_log.spot_name = value;
                if (field == "spot_duration")
                    comm_log.spot_duration = stod(value);
                if (field == "booking_status")
                    comm_log.booking_status = value;

                if (field == "play_date")
                    comm_log.play_date = QDate::fromString(stoq(value), "yyyy-MM-dd");

                if (field == "play_time")
                    comm_log.play_time = QTime::fromString(stoq(value));

                if (field == "booking_id")
                    comm_log.id = std::stoi(value);

                if (field == "schedule_date")
                    comm_log.schedule_date = QDate::fromString(stoq(value), "yyyy-MM-dd");

                if (field == "schedule_time")
                    comm_log.schedule_time = QTime::fromString(stoq(value));
            }

            std::string log_key = comm_log.schedule_time.toString("HH:mm").toStdString();
            m_comm_logs[log_key].push_back(comm_log);
            provider->cache()->next();

        } while(!provider->cache()->isLast());
    }

    if (m_comm_logs.size() > 0)
    {
        CommLogTreeViewModel* comm_log_tvm = new CommLogTreeViewModel(m_comm_logs);
        ui->tvCommLog->setModel(comm_log_tvm);

        ui->tvCommLog->header()->setSectionResizeMode(QHeaderView::Interactive);
        ui->cbExpand->setChecked(true);
    }

}

void CommLogForm::print_log()
{
    //execute_report();
    //return;

    QJsonArray breaks;

    for (auto& [time, commlogs]: m_comm_logs) {
        QJsonArray logs;

        for (auto& commlog: commlogs) {
            QJsonObject log;

            log["client_name"] = QString::fromStdString(commlog.client_name);
            log["spot_id"] = commlog.spot_id;
            log["spot_name"] = QString::fromStdString(commlog.spot_name);
            log["spot_duration"] = commlog.spot_duration;
            log["book_status"] = QString::fromStdString(commlog.booking_status);
            log["play_date"] =  commlog.play_date.toString("dd-MM-yyyy");
            log["play_time"] = commlog.play_time.toString("HH:mm");
            log["schedule_date"] = commlog.schedule_date.toString("dd-MM-yyyy");
            log["schedule_time"] = commlog.schedule_time.toString("HH:mm");

            logs.append(log);

        }

        QJsonObject comm_break;
        comm_break["break_time"] = QString::fromStdString(time);
        comm_break["comms"] = logs;


       breaks.append(comm_break);
    }

    //execute_report();


    if (breaks.size() > 0) {
        write_json_data(breaks);
        // Call report renderer
        execute_report();
    }
}

void CommLogForm::write_json_data(QJsonArray& data)
{
    //QFile file("report_data.json");
    QFile file("D:\\Home\\Lab\\Javascript\\electjson\\data\\report_data.json");
    if (!file.open(QIODevice::ReadWrite)) {
        qDebug() << "File open error";
    }

    QJsonDocument doc;
    doc.setArray(data);

    file.resize(0);
    file.write(doc.toJson());
    file.close();
}

void CommLogForm::execute_report()
{
    //std::wstring exe = "electron";
    QStringList args;
    args << " d:\\home\\lab\\Javascript\\electjson\\main.js breaks";

    QString path = "C:\\Users\\Administrator\\AppData\\Roaming\\npm\\node_modules\\electron\\dist\\electron.exe";

    QString report_launcher = "report.bat";

    QString target = QString("%1 %2").arg(path).arg(args.join(""));

    qDebug() << "Creating report launcher...";

    write_report_launcher(target, report_launcher);

    qDebug() << "Running report...";

    m_process->start(report_launcher);

}


void CommLogForm::write_report_launcher(QString data, QString& launcher)
{
    std::ofstream batch_file;
    batch_file.open(launcher.toStdString());
    if (batch_file.is_open()) {
        batch_file << data.toStdString() << '\n';
        batch_file.close();
    }

}


void CommLogForm::proc_started()
{
    qDebug() << "Process started... thank you.";

}

void CommLogForm::error_occured(QProcess::ProcessError error)
{
    switch(error)
    {
    case QProcess::FailedToStart:
        qDebug() << "Process *FAILED* to start!";
        break;
    case QProcess::Crashed:
        qDebug() << "Process *CRASHED*";
        break;
    case QProcess::Timedout:
        qDebug() << "Process *TIMEDOUT* ";
        break;
    default:
        qDebug() << "Unkown ERROR !!";
        break;
    }

}

/*
void CommLogForm::create_process()
{
    // STARTUPINFO si;
    // PROCESS_INFORMATION pi;

    // ZeroMemory(&si, sizeof(si));
    // si.cb = sizeof(si);
    // ZeroMemory(&pi, sizeof(pi));

    //const TCHAR* target = TEXT("C:\\Windows\\System32\\calc.exe");

    /*
    const TCHAR* target = TEXT("C:\\Users\\Administrator\\AppData\\Roaming\\npm\\node_modules\\electron\\dist\\electron.exe");

    if (!CreateProcess(
        target,           // No module name (use command line)
        NULL,      // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory
        &si,            // Pointer to STARTUPINFO structure
        &pi)            // Pointer to PROCESS_INFORMATION structure
        ) {

        qDebug() << "CreateProcess failed! (" << GetLastError() <<") ";
        }else {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }

}
*/
