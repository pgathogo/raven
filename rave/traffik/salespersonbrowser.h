#ifndef SALESPERSONBROWSER_H
#define SALESPERSONBROWSER_H

#include <QDialog>

#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;
class SalesPerson;
class SalesPersonForm;

namespace Ui {
class SalesPersonBrowser;
}

class SalesPersonBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit SalesPersonBrowser(QWidget *parent = nullptr);
    ~SalesPersonBrowser() override;

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::SalesPersonBrowser *ui;
    SalesPerson* mSalesPerson;
    SalesPersonForm* mSalesPersonForm;
};

#endif // SALESPERSONBROWSER_H
