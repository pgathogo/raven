#include <QApplication>
#include <QMdiArea>

#include "mergedbrowser.h"
#include "ui_mergedbrowser.h"

#include "client.h"
#include "clientbrowser.h"
#include "orderbrowser.h"
#include "spotbrowser.h"
#include "brandbrowser.h"
#include "bookingorderbrowser.h"

MergedBrowser::MergedBrowser(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::MergedBrowser)
    ,m_mdi_area2{nullptr}
    ,m_client{nullptr}
    ,m_client_browser{nullptr}
    ,m_order_browser{nullptr}
    ,m_spot_browser{nullptr}
{
    ui->setupUi(this);

    ui->tabMain->setCurrentIndex(0);

    m_client_browser = std::make_unique<ClientBrowser>(this);
    ui->vlClient->addWidget(m_client_browser.get());

    m_order_browser = std::make_unique<OrderBrowser>(std::make_shared<Client>(), this);
    ui->vlOrder->addWidget(m_order_browser.get());

    m_spot_browser = std::make_unique<SpotBrowser>(std::make_shared<Client>(),
                                                   this);
    ui->vlSpot->addWidget(m_spot_browser.get());

    m_brand_browser = std::make_unique<BrandBrowser>(std::make_shared<Client>(), this);
    ui->vlBrand->addWidget(m_brand_browser.get());

    m_book_order_browser = std::make_unique<BookingOrderBrowser>(this);
    ui->vlBooking->addWidget(m_book_order_browser.get());

    ui->lblTitle->setText("");

    connect(ui->tabMain, &QTabWidget::currentChanged, this, &MergedBrowser::change_tab);

    connect(m_client_browser.get(), &ClientBrowser::client_selection_changed, this, &MergedBrowser::change_client);

    setWindowIcon(QIcon(":/images/icons/images/icons/clients.png"));
    setWindowTitle("Clients Data");

    setMinimumSize(800, 500);

    //ui->tabMain->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

}

void MergedBrowser::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area2 = mdi;
}

MergedBrowser::~MergedBrowser()
{
    delete ui;
}

void MergedBrowser::change_tab(int index)
{
    std::shared_ptr<BaseEntity> ent = m_client_browser->findSelectedEntity();

    if (ent == nullptr)
        return;

    m_client = dynamic_pointer_cast<Client>(ent);

    switch(index){
      case 1:
        m_order_browser->search_by_client(m_client);
        break;
      case 2:
        m_spot_browser->search_by_client(m_client);
        break;
      case 3:
        m_brand_browser->search_by_client(m_client);
        break;
      case 4:
        m_book_order_browser->search_by_client(m_client);
        break;
    }

}

void MergedBrowser::change_client(std::shared_ptr<BaseEntity> entity)
{
    std::shared_ptr<Client> client = dynamic_pointer_cast<Client>(entity);

    m_order_browser->set_client(client);
    m_spot_browser->set_client(client);
    m_brand_browser->set_client(client);

    ui->lblTitle->setText(QString::fromStdString(client->name()->value()));
}

