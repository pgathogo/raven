#ifndef TYPEEXCLUSIONBROWSER_H
#define TYPEEXCLUSIONBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class TypeExclusionBrowser;
}

class TypeExclusionBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "TypeExclusionBrowser";

    explicit TypeExclusionBrowser(QWidget *parent = nullptr);
    ~TypeExclusionBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    std::string typeID() override;

private:
    Ui::TypeExclusionBrowser *ui;

};

#endif // TYPEEXCLUSIONBROWSER_H
