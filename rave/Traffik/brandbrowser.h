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
    explicit BrandBrowser(std::shared_ptr<Client> client, QWidget *parent = nullptr);
    ~BrandBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

    void search_by_client(std::shared_ptr<Client>);

    bool okay_to_delete(std::shared_ptr<BaseEntity> entity) override;

    void set_client(std::shared_ptr<Client> client);

private:
    Ui::BrandBrowser *ui;
    std::shared_ptr<Client> m_client;
};

#endif // BRANDBROWSER_H
