#ifndef CONTENTBROWSER_H
#define CONTENTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class ContentBrowser;
}


class ContentBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ContentBrowser(QWidget* parent = nullptr);
    ~ContentBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    //void searchRecord() override;
private:
    Ui::ContentBrowser *ui;
};

#endif // CONTENTBROWSER_H
