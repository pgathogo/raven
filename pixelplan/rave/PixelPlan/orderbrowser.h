#ifndef ORDERBROWSER_H
#define ORDERBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

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
    bool okay_to_delete(std::shared_ptr<BaseEntity> entity) override;

    void search_by_client(Client*);

    void set_client(Client* client);
private:
    Ui::OrderBrowser *ui;
    Client* mClient;
};

#endif // ORDERBROWSER_H
