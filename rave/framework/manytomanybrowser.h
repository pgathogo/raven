#ifndef MANYTOMANYBROWSER_H
#define MANYTOMANYBROWSER_H

#include <QWidget>

#include "baseentitybrowserdlg.h"
#include "manytomany.h"
//#include "typeexclusion.h"

class EntityDataModel;
class QVBoxLayout;
class PickListBrowser;
class IntegerField;

namespace Ui {
class ManyToManyBrowser;
}


class ManyToManyBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "ManyToManyBrowser";

    explicit ManyToManyBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~ManyToManyBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    std::string typeID() override;

    size_t cnt(){ return mMtoM->getSize(); }

private slots:
    void selectedItem(BaseEntity* entity);

private:
    Ui::ManyToManyBrowser *ui;
    PickListBrowser* plb;
    ManyToMany* mMtoM;
};

#endif // MANYTOMANYBROWSER_H
