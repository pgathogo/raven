#ifndef ORDERBOOKINGBROWSER_H
#define ORDERBOOKINGBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class OrderBookingBrowser;
}

class EntityDataModel;

class OrderBookingBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit OrderBookingBrowser(QWidget *parent = nullptr);
    ~OrderBookingBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;
    void searchRecord() override;

    void addBookingButton();

private slots:
    void newBooking();

private:
    Ui::OrderBookingBrowser *ui;
    std::unique_ptr<EntityDataModel> setupEDM;
};

#endif // ORDERBOOKINGBROWSER_H
