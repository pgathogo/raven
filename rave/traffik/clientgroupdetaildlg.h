#ifndef CLIENTGROUPDETAILDLG_H
#define CLIENTGROUPDETAILDLG_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

class ClientGroup;

namespace Ui {
class ClientGroupDetailDlg;
}

class ClientGroupDetailDlg : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ClientGroupDetailDlg(ClientGroup* cg, QDialog *parent = nullptr);
    ~ClientGroupDetailDlg() override;

    ActionResult saveRecord() override;
    void bindWidgets() override;

    std::string windowTitle() override;

private:
    Ui::ClientGroupDetailDlg *ui;
    ClientGroup* clientGroup;
};

#endif // CLIENTGROUPDETAILDLG_H
