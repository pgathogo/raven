#pragma once

#include <QDialog>
#include <QItemSelection>
#include <memory>

class QMdiArea;
class ClientBrowser;
class OrderBrowser;
class SpotBrowser;
class BrandBrowser;
class Client;
class BookingOrderBrowser;
class BaseEntity;
class QApplication;

namespace Ui {
class MergedBrowser;
}

class MergedBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit MergedBrowser(QWidget* parent = nullptr);
    ~MergedBrowser();

    void setMdiArea(QMdiArea* mdi);

public slots:
    void change_tab(int);
    void change_client(std::shared_ptr<BaseEntity>);

private:
    Ui::MergedBrowser *ui;
    QMdiArea* m_mdi_area2;

    std::shared_ptr<Client> m_client;

    std::unique_ptr<ClientBrowser> m_client_browser;
    std::unique_ptr<OrderBrowser> m_order_browser;
    std::unique_ptr<SpotBrowser> m_spot_browser;
    std::unique_ptr<BrandBrowser> m_brand_browser;

    std::unique_ptr<BookingOrderBrowser> m_book_order_browser;
};

