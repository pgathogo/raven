#ifndef VALUELISTBROWSER_H
#define VALUELISTBROWSER_H

#include <QDialog>
#include "baseentitybrowserdlg.h"

class EntityDataModel;
class ValueListDetailDlg;


namespace Ui {
class ValueListBrowser;
}

class ValueListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    //explicit ValueListBrowser(QWidget *parent = nullptr);
    ValueListBrowser(QWidget* parent = nullptr, ValueList* entity = nullptr);
    ~ValueListBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    //std::string tableName() const;

protected:
    ValueList* mEntity;

private:
    Ui::ValueListBrowser *ui;
    ValueListDetailDlg* valueListDetailDlg;

};

#endif // VALUELISTBROWSER_H
