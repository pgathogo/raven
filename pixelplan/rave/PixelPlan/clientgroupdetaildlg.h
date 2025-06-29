#ifndef CLIENTGROUPDETAILDLG_H
#define CLIENTGROUPDETAILDLG_H

#include <QDialog>
#include "../../../rave/framework/baseentitydetaildlg.h"

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
    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

    void clear_widgets() override;

private:
    Ui::ClientGroupDetailDlg *ui;
    ClientGroup* clientGroup;
};

#endif // CLIENTGROUPDETAILDLG_H
