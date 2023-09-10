
#ifndef STRUCTS_H
#define STRUCTS_H

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
};

struct ConnInfo{
    std::string host{""};
    std::string username{""};
    std::string password{""};
    std::string ip_address{""};
    int port{-1};
    std::string db_name{""};
};

#endif
