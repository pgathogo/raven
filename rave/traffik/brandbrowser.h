#ifndef BRANDBROWSER_H
#define BRANDBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class BrandBrowser;
}

class Client;

class BrandBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit BrandBrowser(Client* client, QWidget *parent = nullptr);
    ~BrandBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

    void search_by_client(Client*);

    bool okay_to_delete(BaseEntity* entity) override;

private:
    Ui::BrandBrowser *ui;
    Client* mClient;
};

#endif // BRANDBROWSER_H
