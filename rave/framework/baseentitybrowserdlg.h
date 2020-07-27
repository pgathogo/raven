#ifndef BASEENTITYBROWSERDLG_H
#define BASEENTITYBROWSERDLG_H

#include <QDialog>
#include <QMdiArea>
#include "entitydatamodel.h"
#include "ui_baseentitybrowserdlg.h"

#include "../framework/ravenexception.h"
#include "../utils/tools.h"

class BaseEntity;
class ValueList;
class QTableView;

namespace Ui {
class BaseEntityBrowserDlg;
}

template<typename T1, typename T2>
std::tuple<T1, T2> searchItem(T1 col, T2 val)
{
    return std::make_tuple(col, val);
}

class BaseEntityBrowserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BaseEntityBrowserDlg(QWidget *parent = nullptr);
    BaseEntityBrowserDlg(QWidget* parent,
                         std::unique_ptr<BaseEntity> entity);
    ~BaseEntityBrowserDlg();

    void setDialogTitle(const QString title);
    void connectSlots();

    virtual void addRecord()=0;
    virtual void updateRecord()=0;
    virtual void deleteRecord();
    virtual void searchRecord();

    virtual std::string typeID();

    template <typename T1, typename T2>
    std::unique_ptr<T2> update()
    {
        std::unique_ptr<T2> dlg(nullptr);

        std::string searchName = selectedRowName().toStdString();
        if (!searchName.empty()){
            BaseEntity* be = mEntityDataModel->findEntityByName(searchName);
            if (be != nullptr){
                T1* entity = dynamic_cast<T1*>(be);
                dlg = std::make_unique<T2>(entity);
                if(dlg->exec() > 0){
                    try{
                        updateTableViewRecord(entity->tableViewValues());
                        mEntityDataModel->updateEntity(*entity);
                    }
                    catch(DatabaseException& de){
                        showMessage(de.errorMessage());
                    }
                }else{
                    dlg = nullptr;
                }
            }
        }

        return std::move(dlg);  // Nasty - never move from local
    }

    EntityDataModel& entityDataModel() const;
    void setMdiArea(QMdiArea* mdi);
protected:
    QMdiArea* mMdiArea;
    int selectedRowId() const;
    QString selectedRowName();
    void updateTableViewRecord(const std::vector<std::string> values);
    void removeSelectedRow();
    void hideAddButton();
    void hideEditButton();
    void hideDeleteButton();
    BaseEntity* findSelectedEntity();
    Ui::BaseEntityBrowserDlg* bui;

public slots:
    void addBtnClicked();
    void editBtnClicked();
    void deleteBtnClicked();
    void searchBtnClicked();

private:
    std::unique_ptr<EntityDataModel> mEntityDataModel;
    void populateFilterCombo();
};

#endif // BASEENTITYBROWSERDLG_H
