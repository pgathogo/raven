#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <random>
#include <sstream>

#include <QString>
#include <QMessageBox>
#include <QDebug>

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

inline void printstr(std::string s)
{
    qDebug() << stoq(s);
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
