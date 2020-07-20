#ifndef BRANDBROWSER_H
#define BRANDBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class BrandBrowser;
}

class BrandBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit BrandBrowser(QWidget *parent = nullptr);
    ~BrandBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

private:
    Ui::BrandBrowser *ui;
};

#endif // BRANDBROWSER_H
