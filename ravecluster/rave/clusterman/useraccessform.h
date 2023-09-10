#ifndef USERACCESSFORM_H
#define USERACCESSFORM_H

#include <QDialog>
#include "../security/structs.h"

class QListWidgetItem;

enum class AccessAction{None, New, Delete};

struct StationAccess{
    int id{-1};
    int station_id{-1};
    int cluster_id{-1};
    QString name;
    AccessAction status;
};

struct StationData{
    int station_id{-1};
    int cluster_id{-1};
    QString station_name;
    QString db_name;
};


namespace SECURITY{
    class User;
}

namespace Ui {
class UserAccessForm;
}

namespace ClusterManager{
class ClusterConfigurationManager;
}

class UserAccessForm : public QDialog
{
    Q_OBJECT

public:
    explicit UserAccessForm(SECURITY::User* user, QWidget *parent = nullptr);
    std::map<int, StationAccess> user_access();
    ~UserAccessForm();

private slots:
    void station_selected(QListWidgetItem*);
    void save();

private:
    void load_stations();
    void load_user_stations(std::string);
    void setup_gui();

    void add_station();
    void remove_station();

    void add_access(int, int, QString);
    void remove_access(int);

    void print_station_access();
    ConnInfo find_db_server(QString, int);

    Ui::UserAccessForm *ui;
    SECURITY::User* m_user;

    std::map<int, StationAccess> m_user_access;
    std::map<int, StationData> m_station_data;

    std::unique_ptr<ClusterManager::ClusterConfigurationManager> m_ccm;


};

#endif // USERACCESSFORM_H
