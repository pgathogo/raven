#ifndef ORDERBROWSER_H
#define ORDERBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class OrderBrowser;
}

class Client;

class OrderBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit OrderBrowser(Client* client, QWidget* parent = nullptr);
    ~OrderBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;
    bool okay_to_delete(BaseEntity* entity) override;

private:
    Ui::OrderBrowser *ui;
    Client* mClient;
};

#endif // ORDERBROWSER_H
