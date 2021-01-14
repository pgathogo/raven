#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <random>
#include <sstream>
#include <memory>

#include <QString>
#include <QMessageBox>
#include <QDebug>

#include <cstdlib>
#include <cxxabi.h>

inline QString stoq(std::string s)
{
    return QString::fromStdString(s);
}

inline void showMessage(std::string msg)
{
    QMessageBox msgBox;
    msgBox.setText(stoq(msg));
    msgBox.exec();
}

inline bool confirmationMessage(const std::string msg)
{
    bool result = false;

    QMessageBox msgBox;
    msgBox.setText("Confirmation Message");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setInformativeText(stoq(msg));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    switch (ret){
        case QMessageBox::Ok:
        result = true;
        break;
        case QMessageBox::Cancel:
        result = false;
        break;
        default:
        result = false;
        break;
    }

    return result;
}


inline void printstr(std::string s)
{
    qDebug() << "STR: >> " << stoq(s);
}

inline void printint(int i)
{
    qDebug() << "INT: >> " << i;
}

inline void printdouble(double d)
{
    qDebug() << "DBL: >> " << d;
}

inline int str_to_int(std::string s){
    return (s.empty()) ? 0 : std::stoi(s);
}

inline double str_to_double(std::string s){
    return (s.empty()) ? 0.0 : std::stod(s);
}


inline std::tuple<int, int, int> ymd(const std::string date_str)
{
        char  yr[5];
        char mth[3];
        char day[3];

        date_str.copy(yr, 4, 0);
        date_str.copy(mth, 2, 5);
        date_str.copy(day, 2, 8);

        return std::make_tuple(std::atoi(yr), std::atoi(mth),
                               std::atoi(day));
}
inline std::string demangle(const char* name){
    int status = 4; // some abitrary value to eliminate the compiler warning

    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

    return (status == 0) ? res.get() : name ;
}

namespace uuid {
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    inline std::string generate_uuid_v4() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        }
        return ss.str();
    }
}

#endif // TOOLS_H
