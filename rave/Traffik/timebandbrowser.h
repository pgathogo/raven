#ifndef TIMEBANDBROWSER_H
#define TIMEBANDBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class TimeBandBrowser;
}

class TimeBandBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit TimeBandBrowser(QWidget *parent = nullptr);
    ~TimeBandBrowser();

    void addRecord() override;
    void updateRecord() override;
    std::string typeID() override;

private:
    Ui::TimeBandBrowser *ui;
};

#endif // TIMEBANDBROWSER_H
