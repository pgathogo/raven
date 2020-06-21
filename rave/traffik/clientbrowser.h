#ifndef CLIENTBROWSER_H
#define CLIENTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class ClientBrowser;
}

class ClientBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "ClientBrowser";
    explicit ClientBrowser(QWidget *parent = nullptr);
    ~ClientBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    std::string typeID() override;

private:
    Ui::ClientBrowser *ui;
};

#endif // CLIENTBROWSER_H
