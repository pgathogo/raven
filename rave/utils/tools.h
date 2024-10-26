#ifndef TOOLS_H
#define TOOLS_H

#include <Windows.h>
#include <DbgHelp.h>
#include <chrono>

#include <string>
#include <random>
#include <sstream>
#include <memory>

#include <QString>
#include <QMessageBox>
#include <QDebug>

#include <cstdlib>

#if defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#endif

//#pragma comment(lib, "DbgHelp.Lib")

//extern char *__unDName(char*, const char*, int, void*, void*, int);


inline void timer()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();

    auto t2 = high_resolution_clock::now();

    auto ms_int = duration_cast<milliseconds>(t2-t1);

    duration<double, std::milli> ms_double = t2 - t1;

//    std::cout << ms_int.count() << "ms\n";
//    std::cout << ms_double.count() << "ms\n";
}


inline QString stoq(std::string s)
{
    return QString::fromStdString(s);
}

inline void showMessage(std::string msg, QMessageBox::Icon icon = QMessageBox::Information)
{
    QMessageBox msgBox;
    msgBox.setText(stoq(msg));
    msgBox.setIcon(icon);
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

inline void dbgln(const QString msg)
{
    qDebug() << msg;
}


inline void printstr(std::string s)
{
    qDebug() << stoq(s);
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

inline std::vector<std::string> split_string(std::string source, char delim)
{
    std::vector<std::string> results;
    std::stringstream ss(source);
    std::string item;

    while(getline(ss, item, delim)){
        results.push_back(item);
    }
    return results;
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

inline std::string encrypt_str(std::string str)
{
        for (int i=0; (i < 100 && str[i] != '\0'); ++i)
        {
            str[i] = str[i] + 2;
        }

        return str;
}

inline std::string decrypt_str(std::string str)
{
        for(int i=0; ( i < 100 && str[i] != '\0'); ++i){
            str[i] = str[i] - 2;
        }
        return str;
}


#if defined(__GNUC__) || defined(__GNUG__)
inline std::string demangle(const char* name){
    int status = 4; // some abitrary value to eliminate the compiler warning

    std::unique_ptr<char, void (*)(void *)> res{
        abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

    return (status == 0) ? res.get() : name ;
}
#endif

#if defined(_MSC_VER)
inline std::string demangle(const char* name){
    //const char *decorated_name = 0;
    char undecorated_name[1024];
    //__unDName(undecorated_name, name+1, 1024, malloc, free, 0x2800);
    UnDecorateSymbolName(name, undecorated_name, sizeof(name)/sizeof(*name), UNDNAME_COMPLETE);

    std::string formatted_str = undecorated_name;
    return formatted_str;

}
#endif


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

