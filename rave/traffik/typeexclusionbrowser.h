#ifndef TYPEEXCLUSIONBROWSER_H
#define TYPEEXCLUSIONBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class TypeExclusionBrowser;
}

class TypeExclusionBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit TypeExclusionBrowser(QWidget *parent = nullptr);
    ~TypeExclusionBrowser() override;

    void addRecord() override;
    void updateRecord() override;

private:
    Ui::TypeExclusionBrowser *ui;
};

#endif // TYPEEXCLUSIONBROWSER_H
