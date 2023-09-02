#ifndef USERACCESSFORM_H
#define USERACCESSFORM_H

#include <QDialog>

class QListWidgetItem;

enum class AccessAction{None, New, Delete};

struct StationAccess{
    int id{-1};
    int station_id{-1};
    QString name;
    AccessAction status;
};

namespace SECURITY{
    class User;
}

namespace Ui {
class UserAccessForm;
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

    void add_access(int, QString);
    void remove_access(int);

    void print_station_access();

    Ui::UserAccessForm *ui;
    SECURITY::User* m_user;

    std::map<int, StationAccess> m_user_access;


};

#endif // USERACCESSFORM_H
