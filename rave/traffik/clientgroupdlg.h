#ifndef CLIENTGROUPDLG_H
#define CLIENTGROUPDLG_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;
class ClientGroupDetailDlg;

namespace Ui {
class ClientGroupDlg;
}

class ClientGroupDlg : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ClientGroupDlg(QWidget *parent = nullptr);
    ~ClientGroupDlg() override;

    void AddRecord() override;
    void searchRecord() override;

private:
    Ui::ClientGroupDlg *ui;
    ClientGroupDetailDlg* clientGroupDetailDlg;
};

#endif // CLIENTGROUPDLG_H
