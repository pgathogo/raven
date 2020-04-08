#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QMessageBox>
#include <string>

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

#endif // TOOLS_H
