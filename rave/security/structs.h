
#ifndef STRUCTS_H
#define STRUCTS_H

#include <ostream>
#include <QString>

struct StationInfo{
    int server_id;
    QString server_name;
    QString ip_address;
    int port_no;
    int station_id;
    QString station_name;
    QString db_name;
    QString cluster_name;
    int cluster_id;

    friend std::ostream& operator<<(std::ostream& os, const StationInfo& si);
};

inline std::ostream& operator<<(std::ostream& os, const StationInfo& si) {
    os << "Server ID: " << si.server_id << '\n';
    os << "Server Name: " << si.server_name.toStdString() << '\n';
    os << "IP Address: " << si.ip_address.toStdString() << '\n';
    os << "Port No: " << si.port_no << '\n';
    os << "Station Name: " << si.station_name.toStdString() << '\n';
    os << "DB Name: " << si.db_name.toStdString() << '\n';
    os << "Cluster Name: " << si.cluster_name.toStdString() << '\n';
    os << "Cluster ID: " << si.cluster_id << '\n';
    os << "Server ID: " << si.server_id << '\n';
}


struct ConnInfo{
    std::string host{""};
    std::string username{""};
    std::string password{""};
    std::string ip_address{""};
    int port{-1};
    std::string db_name{""};

    friend std::ostream& operator<<(std::ostream& os, const ConnInfo& ci);

};

inline std::ostream& operator<<(std::ostream& os, const ConnInfo& ci) {
    os << "Host: "<< ci.host << '\n';
    os << "Username: " << ci.username << '\n';
    os << "Password: " << ci.password << '\n';
    os << "IP Address: "<< ci.ip_address << '\n';
    os << "Port: "<< ci.port << '\n';
    os << "DB Name: "<< ci.db_name << '\n';
    return os;
}

#endif
