#include <sstream>

#include <QCloseEvent>
#include "loginform.h"
#include "ui_loginform.h"

#include "../utils/notificationbar.h"
#include "../framework/ravenexception.h"
#include "../framework/entitydatamodel.h"
#include "../security/userconfig.h"
#include "../../ravecluster/rave/clusterman/clustercontroller.h"

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

    connect(ui->btnReset, &QPushButton::clicked, this, &LoginForm::reset_password);
    connect(ui->btnCancelReset, &QPushButton::clicked, this, &LoginForm::cancel_password_reset);

    mNoticeBar = new NotificationBar(ui->vlNotice);
    m_reset_notice_bar = new NotificationBar(ui->vlReset);

    ui->edtUsername->setFocusPolicy(Qt::StrongFocus);
    ui->edtUsername->setFocus();

    //ui->btnLogin->setEnabled(false);

    ui->swMain->setCurrentIndex(0);

    setWindowTitle("Login Form");

}

LoginForm::~LoginForm()
{
    delete ui;
    delete mNoticeBar;
    delete m_reset_notice_bar;
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
    auto app_auth = std::make_unique<Authentication>();

    try{
        app_auth->connect_to_cluster_server(
            ui->edtUsername->text().toStdString(),
            ui->edtPassword->text().toStdString());
    }catch(DatabaseException& de){
        std::cout << de.errorMessage();
    }

    SECURITY::UserConfig uc;
    EntityDataModel edm(std::make_unique<SECURITY::UserConfig>());
    auto user_filter = std::make_tuple(uc.username()->dbColumnName(),"=", "'"+ui->edtUsername->text().toStdString()+"'");
    std::string filter = edm.prepareFilter(user_filter);

    edm.search(filter);

    if (edm.modelEntities().size() > 0)
    {
        auto const& [name, entity] = edm.modelEntities()[0];
        auto ucptr = dynamic_cast<SECURITY::UserConfig*>(entity.get());

        int can_reset_password = ucptr->reset_password()->value();
        if (can_reset_password == 1)
        {
            ui->lblUsername->setText(ui->edtUsername->text());
            ui->swMain->setCurrentIndex(can_reset_password);
            return;
        }
    }

    m_selected_station_id = get_selected_station_id();
    if (m_selected_station_id > 0)
    {
        auto si = m_stations_info[m_selected_station_id];
        ui->btnSelect->setText(si.station_name);
        ui->btnLogin->setEnabled(true);
    }
}

int LoginForm::get_selected_station_id()
{
    bool populated = populate_station_info(ui->edtUsername->text());
    if (!populated)
        return 0;

    auto ssform = std::make_unique<SelectStationForm>(m_stations_info);
    if (ssform->exec() > 0)
    {
        return  ssform->station_id();
    }else{
        return 0;
    }
}


void LoginForm::login()
{
    if (!validNamePass())
    {
        mNoticeBar->errorNotification("Wrong username or password!");
        mOkClose = false;
        return;
    }

    if (m_selected_station_id < 0 ){
        mNoticeBar->errorNotification("Please select a station!");
        return;
    }

    login_to_station(m_selected_station_id);

}

void LoginForm::login_to_station(int station_id)
{
    try
    {
        m_current_station_info = m_stations_info[m_selected_station_id];

        //ConnInfo ci;
        m_current_conn_info.username = ui->edtUsername->text().toStdString();
        m_current_conn_info.password = ui->edtPassword->text().toStdString();
        m_current_conn_info.host = m_current_station_info.ip_address.toStdString();
        m_current_conn_info.port = m_current_station_info.port_no;
        m_current_conn_info.db_name = m_current_station_info.db_name.toStdString();

        std::cout << "Username: "<< m_current_conn_info.username << '\n';
        std::cout << "Password: "<< m_current_conn_info.password << '\n';
        std::cout << "Host: "<< m_current_conn_info.host << '\n';
        std::cout << "Port: "<< m_current_conn_info.port << '\n';
        std::cout << "Database: "<< m_current_conn_info.db_name << '\n';

        m_app_auth = new Authentication(m_current_conn_info);
        m_app_auth->connect_to_server();
//        m_app_auth->connect_to_station(m_current_conn_info);
//        m_app_auth->connect("herman", "abc123");

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

void LoginForm::reset_password()
{
    if (validate_password_reset())
    {
        ClusterManager::ClusterController cluster_controller;
        std::string username = ui->edtUsername->text().toStdString();
        std::string password = ui->edtNewPassword->text().toStdString();

         cluster_controller.alter_password(username, password);

        // Switch back to cluster server
        Authentication auth;
        ConnInfo ci = auth.cluster_server_conninfo();
        ci.username = username;
        ci.password = password;

        try{
            Authentication auth2(ci);
            auth2.connect_to_server();
             cluster_controller.remove_password_reset_flag(username);
            m_selected_station_id = get_selected_station_id();

        }catch(DatabaseException& de){
            std::cout << de.errorMessage();
        }

        if (m_selected_station_id > 0)
        {
            login_to_station(m_selected_station_id);
            mOkClose = true;
            done(1);
        }
    }
}

void LoginForm::cancel_password_reset()
{
    done(0);
}

bool LoginForm::validate_password_reset()
{
    if (ui->edtNewPassword->text().isEmpty()){
        m_reset_notice_bar->errorNotification("Enter new password!");
        mOkClose = false;
        return false;
    }

    if(ui->edtConfirmPassword->text().isEmpty()){
        m_reset_notice_bar->errorNotification("Confirm new password!");
        mOkClose = false;
        return false;
    }

    if (ui->edtNewPassword->text() != ui->edtConfirmPassword->text()){
        m_reset_notice_bar->errorNotification("Password not matching!");
        mOkClose = false;
        return false;
    }

    return true;
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

