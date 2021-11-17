#ifndef CLIENTGROUPDLG_H
#define CLIENTGROUPDLG_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class ClientGroupDlg;
}

class ClientGroupDlg : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "ClientGroupDlg";

    explicit ClientGroupDlg(QWidget *parent = nullptr);
    ~ClientGroupDlg() override;

    void addRecord() override;
    void updateRecord() override;

    std::string typeID() override;
private:

    Ui::ClientGroupDlg *ui;
};

#endif // CLIENTGROUPDLG_H
