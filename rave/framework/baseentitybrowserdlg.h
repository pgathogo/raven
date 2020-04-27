#ifndef BASEENTITYBROWSERDLG_H
#define BASEENTITYBROWSERDLG_H

#include <QDialog>
#include <QMdiArea>
#include "entitydatamodel.h"

class BaseEntity;
class ValueList;
class QTableView;

namespace Ui {
class BaseEntityBrowserDlg;
}

class BaseEntityBrowserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BaseEntityBrowserDlg(QWidget *parent = nullptr);
    BaseEntityBrowserDlg(QWidget* parent, BaseEntity* entity);
    ~BaseEntityBrowserDlg();

    void setDialogTitle(const QString title);
    void connectSlots();
    void setMdiArea(QMdiArea* mdi);

    virtual void addRecord()=0;
    virtual void updateRecord()=0;
    virtual void deleteRecord();
    virtual void searchRecord();

    void setEntityDataModel(BaseEntity* entity);
    BaseEntity* baseEntity();

    template <typename T1, typename T2>
    void update()
    {
        std::string searchName = selectedRowName().toStdString();
        if (!searchName.empty()){
        BaseEntity* be = mEntityDataModel->findEntityByName(searchName);
        if (be != nullptr){
          T1* entity = dynamic_cast<T1*>(be);
          T2* dlg = new T2(entity);
          if(dlg->exec() > 0){
              updateTableViewRecord(entity);
              mEntityDataModel->updateEntity(entity);
          }
        }

      }
    }

    template <typename T1, typename T2>
    void addEntity()
    {
        std::unique_ptr<T1> uT1 = std::make_unique<T1>();
        auto ptr(uT1.get());
        T2* dlg = new T2(ptr);
        if (dlg->exec() > 0)
            entityDataModel()->createEntity(std::move(uT1));
    }

    template <typename T1, typename T2>
    void addEntity(T1* e)
    {
        std::unique_ptr<T1> uT1 = std::make_unique<T1>();
        auto ptr(uT1.get());
        ptr = e;
        T2* dlg = new T2(ptr);
        if (dlg->exec() > 0){
            std::unique_ptr<T1> uT3(ptr);
            entityDataModel()->createEntity(std::move(uT3));
        }
    }

protected:
    EntityDataModel* entityDataModel() const;
    QMdiArea* mMdiArea;
    int selectedRowId() const;
    QString selectedRowName();
    void updateTableViewRecord(BaseEntity* entity);
    void removeSelectedRow();
    void hideAddButton();
    void hideEditButton();
    void hideDeleteButton();
    Ui::BaseEntityBrowserDlg* bui;

public slots:
    void addBtnClicked();
    void editBtnClicked();
    void deleteBtnClicked();
    void searchBtnClicked();



private:
    BaseEntity* mBaseEntity;
    EntityDataModel* mEntityDataModel;
    void populateFilterCombo();
};

#endif // BASEENTITYBROWSERDLG_H
