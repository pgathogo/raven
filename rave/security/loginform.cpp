#include <sstream>

#include <QCloseEvent>
#include "loginform.h"
#include "ui_loginform.h"

#include "../utils/notificationbar.h"
#include "../framework/ravenexception.h"
#include "../framework/entitydatamodel.h"
#include "user.h"
#include "authentication.h"
#include "selectstationform.h"


LoginForm::LoginForm(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::LoginForm)
    ,m_app_auth{nullptr}
    ,mNoticeBar{}
    ,mOkClose{true}
    ,m_selected_station_id{-1}
{
    ui->setupUi(this);

    connect(ui->btnSelect, &QPushButton::clicked, this, &LoginForm::fetch_stations);
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginForm::login);
    connect(ui->btnCancel, &QPushButton::clicked, this, &LoginForm::cancel);

    mNoticeBar = new NotificationBar(ui->vlNotice);

    ui->edtUsername->setFocusPolicy(Qt::StrongFocus);
    ui->edtUsername->setFocus();
    ui->btnLogin->setEnabled(false);

    setWindowTitle("Login Form");
}

LoginForm::~LoginForm()
{
    delete ui;
    delete mNoticeBar;
    //delete m_app_auth;
}

void LoginForm::closeEvent(QCloseEvent *event)
{
    done(0);
    if(!mOkClose)
        event->ignore();
}

bool LoginForm::validNamePass()
{
    if (ui->edtUsername->text().isEmpty())
        return false;

    if (ui->edtPassword->text().isEmpty())
        return false;

    return true;
}

void LoginForm::fetch_stations()
{

    auto app_auth = new Authentication();
    app_auth->connect_to_cluster_server("postgres", "abc123");

    bool populated = populate_station_info(ui->edtUsername->text());

    auto ssform = std::make_unique<SelectStationForm>(m_stations_info);

    if (ssform->exec() > 0){
        m_selected_station_id = ssform->station_id();
        auto si = m_stations_info[m_selected_station_id];
        ui->btnSelect->setText(si.station_name);

        ui->btnLogin->setEnabled(true);
    }

    delete app_auth;

}

void LoginForm::login()
{

    if (!validNamePass()){
        mNoticeBar->errorNotification("Wrong username or password!");
        mOkClose = false;
        return;
    }

    try{
        m_current_station_info = m_stations_info[m_selected_station_id];

        auto auth = new Authentication();

        //ConnInfo ci;
        m_current_conn_info.username = ui->edtUsername->text().toStdString();
        m_current_conn_info.password = ui->edtPassword->text().toStdString();
        m_current_conn_info.host = m_current_station_info.ip_address.toStdString();
        m_current_conn_info.port = m_current_station_info.port_no;
        m_current_conn_info.db_name = m_current_station_info.db_name.toStdString();

        auth->connect_to_station(m_current_conn_info);

        // Give user "postgres" all access rights.
        //auth->access_controller(ui->edtUsername->text().toStdString());

        mOkClose = true;
        done(1);
    }catch(PostgresException& pe){
        mNoticeBar->errorNotification(pe.errorMessage());
        mOkClose = false;
    }

}

StationInfo LoginForm::get_station_info()
{
    return m_current_station_info;
}

ConnInfo LoginForm::get_connection_info()
{
    return m_current_conn_info;
}


void LoginForm::cancel()
{
    done(0);
}

bool LoginForm::populate_station_info(QString username)
{
    std::stringstream sql;

    std::string user_filter = std::format(" and ua.username = '{}'", username.toStdString());

    sql << " select svr.id AS server_id, svr.server_name, svr.server_ip, svr.port_no, "
        << " st.id AS station_id, st.station_name, st.db_name, st.cluster_id, cl.cluster_name "
        << " from rave_server svr, rave_station st, rave_useraccess ua, rave_cluster cl"
        << " Where svr.cluster_id = st.cluster_id "
        << " and st.id = ua.station_id "
        << " and cl.id = st.cluster_id "
        << " and svr.server_type = 'DBS' "
        << user_filter;


    EntityDataModel edm;

    edm.readRaw(sql.str());


    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return false;


    provider->cache()->first();
    do{
        auto itb = provider->cache()->currentElement()->begin();
        auto ite = provider->cache()->currentElement()->end();

        StationInfo si;

        for (; itb != ite; ++itb)
        {
            std::string field = (*itb).first;
            std::string value = (*itb).second;


            if (field == "server_id")
                si.server_id = std::stoi(value);

            if (field == "server_name" && (!value.empty()))
                si.server_name = stoq(value);

            if (field == "server_ip" && (!value.empty()))
                si.ip_address = stoq(value);

            if (field == "port_no" && (!value.empty()))
                si.port_no = std::stoi(value);

            if (field == "station_id")
                si.station_id = std::stoi(value);

            if (field == "station_name" && (!value.empty()))
                si.station_name = stoq(value);

            if (field == "db_name" && (!value.empty()))
                si.db_name = stoq(value);

            if (field == "cluster_name" && (!value.empty()))
                si.cluster_name = stoq(value);

            if (field == "cluster_id")
                si.cluster_id = std::stoi(value);

        }

        m_stations_info[si.station_id] = si;

        provider->cache()->next();
    }while(!provider->cache()->isLast());

    return true;

}

