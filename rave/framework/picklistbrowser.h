#ifndef PICKLISTBROWSER_H
#define PICKLISTBROWSER_H

#include <QDialog>

#include "baseentitybrowserdlg.h"

class SelectCloseWidget;
class ManyToMany;
class EntityDataModel;

namespace Ui {
class PickListBrowser;
}

struct PickListSetting {
    BaseEntity* listEntity;
    std::vector<BaseEntity*> selectedEntities;
};

class PickListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
                //EntityDataModel* edm,
    explicit PickListBrowser(PickListSetting& plSetting,
                            QWidget *parent = nullptr);
    ~PickListBrowser() override;

    void addRecord() override;
    void updateRecord() override;

public slots:
    void onSelectItem();
    void onCloseSelection();

signals:
    void itemSelected(BaseEntity* entity);

private:
    Ui::PickListBrowser *ui;
    SelectCloseWidget* scw;
    PickListSetting& mPickListSetting;
    //BaseEntity* mEntity;
    //EntityDataModel* destModel;
    //ManyToMany& mMtoM;
    //std::vector<BaseEntity*> cont;
};

#endif // PICKLISTBROWSER_H
