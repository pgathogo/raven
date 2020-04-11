#ifndef TOOLS_H
#define TOOLS_H

#include <string>

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

#endif // TOOLS_H
