#ifndef MANYTOMANYBROWSER_H
#define MANYTOMANYBROWSER_H

#include <QWidget>

#include "baseentitybrowserdlg.h"

class EntityDataModel;
class QVBoxLayout;
class PickListBrowser;

namespace Ui {
class ManyToManyBrowser;
}

class ManyToManyBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit ManyToManyBrowser(QVBoxLayout* layout, QWidget *parent = nullptr);
    ~ManyToManyBrowser() override;

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::ManyToManyBrowser *ui;
    PickListBrowser* plb;
};

#endif // MANYTOMANYBROWSER_H
