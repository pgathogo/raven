#ifndef BASEENTITYBROWSERDLG_H
#define BASEENTITYBROWSERDLG_H

#include <QDebug>

#include <QDialog>
#include <QMdiArea>
#include "entitydatamodel.h"
#include "ui_baseentitybrowserdlg.h"

#include "ravenexception.h"
#include "manytomany.h"
#include "../utils/tools.h"

class BaseEntity;
class ValueList;
class QTableView;

namespace Ui {
class BaseEntityBrowserDlg;
}

using AccessMap = std::map<std::string, std::string>;

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

    void setAccess(std::string access_bit);

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

    template<typename T1, typename T2>
    void rawAdd()
    {
        std::unique_ptr<T1> t1 = std::make_unique<T1>();
        std::unique_ptr<T2> form =
                std::make_unique<T2>(t1.get());
        if (form->exec() > 0){

            std::string create_stmt =
                    t1->make_create_stmt();
            try{
                entityDataModel().executeRawSQL( create_stmt );
                entityDataModel().cacheEntity( std::move(t1) );

            } catch (DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }

    }

    template<typename T>
    class HasParentId
    {
        private:
            typedef char YesType[1];
            typedef char NoType[2];

            template <typename C>
            static YesType& test(decltype(&C::parentId));
            template <typename C>
            static NoType& test(...);
        public:
            enum{ value = sizeof(test<T>(0)) == sizeof(YesType) };
    };

   // typename std::enable_if<HasParentId<T1>::value, int>::type
    template<typename T1>
    void save_detail_item(int parentId,  T1* mtom)
    {
        std::unique_ptr<EntityDataModel> edm =
           std::make_unique<EntityDataModel>();

        if (mtom->dbAction() == DBAction::dbaCREATE){
            mtom->setParentId(parentId);
            auto insert_stmt = edm->make_insert_stmt(*mtom);
            entityDataModel().executeRawSQL( insert_stmt );
            //edm->createEntityDB(*mtom);
        }

        if (mtom->dbAction() == DBAction::dbaDELETE)
            edm->deleteEntity(*mtom);
    }

    int save_detail_item(...){ return 0; }

    template<typename T1, typename T2, typename T3 = void>
    void rawUpdate()
    {
        BaseEntity* entity = findSelectedEntity();
        T1* t1 = dynamic_cast<T1*>(entity);

        std::unique_ptr<T2> form =
                std::make_unique<T2>(t1, this);

        if (form->exec() > 0){
            std::string alter_stmt =
                    t1->make_alter_stmt(t1->displayName());
            try{
                entityDataModel().executeRawSQL( alter_stmt );
                //updateTableViewRecord(t1->tableViewValues());
                //entityDataModel().updateEntity(*t1);

                for(auto& mtomForm : form->getForms()){
                    auto& items = mtomForm->entityDataModel().modelEntities();
                    for (auto&[name, entity] : items){
                        //ManyToMany* mtom = dynamic_cast<ManyToMany*>(entity.get());
                        T3* mtom = dynamic_cast<T3*>(entity.get());
                       if constexpr(HasParentId<T2>::value > 0)
                            save_detail_item<T3>(t1->id(),  mtom);
                    }
                }

            }catch (DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }
    }

    template<typename T1>
    void rawDelete()
    {
        BaseEntity* entity = findSelectedEntity();
        T1* t1 = dynamic_cast<T1*>(entity);

        std::string drop_stmt = t1->make_drop_stmt(
                    t1->displayName());
        try{
            entityDataModel().executeRawSQL( drop_stmt );
            removeSelectedRow();

        } catch (DatabaseException& de) {
            showMessage(de.errorMessage());
        }
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
