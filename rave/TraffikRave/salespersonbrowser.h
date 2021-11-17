#ifndef SALESPERSONBROWSER_H
#define SALESPERSONBROWSER_H

#include <QDialog>

#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class SalesPersonBrowser;
}

class SalesPersonBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "SalesPersonBrowser";

    explicit SalesPersonBrowser(QWidget *parent = nullptr);
    ~SalesPersonBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    std::string typeID() override;

private:
    Ui::SalesPersonBrowser *ui;
};

#endif // SALESPERSONBROWSER_H
