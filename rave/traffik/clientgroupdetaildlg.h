#ifndef CLIENTGROUPDETAILDLG_H
#define CLIENTGROUPDETAILDLG_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class EntityDataModel;

namespace Ui {
class ClientGroupDetailDlg;
}

class ClientGroupDetailDlg : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ClientGroupDetailDlg(EntityDataModel* edm,
                              QWidget *parent = nullptr);
    ~ClientGroupDetailDlg();

    void saveRecord() override;

private:
    Ui::ClientGroupDetailDlg *ui;
};

#endif // CLIENTGROUPDETAILDLG_H
