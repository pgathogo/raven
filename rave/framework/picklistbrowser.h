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

class PickListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit PickListBrowser(ManyToMany& mtom,
                            EntityDataModel* edm,
                            QWidget *parent = nullptr);
    ~PickListBrowser() override;

    void addRecord() override;
    void updateRecord() override;

public slots:
    void onSelectItem();

signals:
    void itemSelected(BaseEntity* entity);

private:
    Ui::PickListBrowser *ui;
    SelectCloseWidget* scw;
    BaseEntity* mEntity;
    EntityDataModel* destModel;
    ManyToMany& mMtoM;
};

#endif // PICKLISTBROWSER_H
