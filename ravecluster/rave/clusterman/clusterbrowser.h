#ifndef CLUSTERBROWSER_H
#define CLUSTERBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

namespace Ui {
class ClusterBrowser;
}


class ClusterBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ClusterBrowser(QWidget *parent = nullptr);
    ~ClusterBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

private:
    Ui::ClusterBrowser *ui;
};

#endif // CLUSTERBROWSER_H
