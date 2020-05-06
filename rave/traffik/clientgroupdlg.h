#ifndef CLIENTGROUPDLG_H
#define CLIENTGROUPDLG_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;
class ClientGroupDetailDlg;
class ClientGroup;

namespace Ui {
class ClientGroupDlg;
}

class ClientGroupDlg : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ClientGroupDlg(QWidget *parent = nullptr, ClientGroup* cg = nullptr);
    ~ClientGroupDlg() override;

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::ClientGroupDlg *ui;
    ClientGroupDetailDlg* clientGroupDetailDlg;
    ClientGroup* clientGroup;
};

#endif // CLIENTGROUPDLG_H
