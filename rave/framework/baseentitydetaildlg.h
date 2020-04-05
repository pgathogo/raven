#ifndef BASEENTITYDETAILDLG_H
#define BASEENTITYDETAILDLG_H

#include <QWidget>

class EntityDataModel;
class BaseEntity;

namespace Ui {
class BaseEntityDetailDlg;
}

class BaseEntityDetailDlg : public QWidget
{
    Q_OBJECT

public:
    explicit BaseEntityDetailDlg(EntityDataModel* edm, QWidget *parent = nullptr);
    ~BaseEntityDetailDlg();

    void connectSlots();
    virtual void saveRecord()=0;

protected:
    Ui::BaseEntityDetailDlg* bui;
    EntityDataModel* entityDataModel();
    void save(BaseEntity* entity);
private:
    EntityDataModel* mEntityDataModel;
};

#endif // BASEENTITYDETAILDLG_H
