#ifndef PICKLISTBROWSER_H
#define PICKLISTBROWSER_H

#include <QDialog>

#include "baseentitybrowserdlg.h"

class SelectCloseWidget;
class EntityDataModel;

namespace Ui {
class PickListBrowser;
}

class PickListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit PickListBrowser(BaseEntity* entity,
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
};

#endif // PICKLISTBROWSER_H
