#ifndef ORDERBROWSER_H
#define ORDERBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

namespace Ui {
class OrderBrowser;
}

namespace PIXELPLAN{
class OrderChangeLog;
}

class Client;
class Order;


class OrderBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit OrderBrowser(std::shared_ptr<Client> client, QWidget* parent = nullptr);
    ~OrderBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;
    bool okay_to_delete(std::shared_ptr<BaseEntity> entity) override;

    void search_by_client(std::shared_ptr<Client>);

    void set_client(std::shared_ptr<Client> client);
private:
    void log_prev_data(std::shared_ptr<PIXELPLAN::OrderChangeLog>, std::shared_ptr<Order>);
    void log_current_data(std::shared_ptr<PIXELPLAN::OrderChangeLog>, std::shared_ptr<Order>);
    int find_order_prev_sequence(std::shared_ptr<Order>);

    Ui::OrderBrowser *ui;
    std::shared_ptr<Client> mClient;
};

#endif // ORDERBROWSER_H
