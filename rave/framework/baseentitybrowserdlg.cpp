#include <QMessageBox>

#include "baseentitybrowserdlg.h"
//#include "ui_baseentitybrowserdlg.h"

#include "baseentity.h"
#include "entitydatamodel.h"

#include "../utils/tools.h"
#include "../framework/ravenexception.h"

BaseEntityBrowserDlg::BaseEntityBrowserDlg( QWidget *parent) :
    QDialog(parent),
    mMdiArea{nullptr},
    bui(new Ui::BaseEntityBrowserDlg),
    mEntityDataModel{nullptr}
{
    bui->setupUi(this);
    connectSlots();
}

BaseEntityBrowserDlg::BaseEntityBrowserDlg(QWidget* parent,
                                           std::unique_ptr<BaseEntity> entity)
        :QDialog(parent)
        ,mMdiArea{nullptr}
        ,bui(new Ui::BaseEntityBrowserDlg)
        ,mEntityDataModel{nullptr}
{
    std::string class_name = demangle(typeid(entity).name());
    qDebug() << QString::fromStdString(class_name);

    mEntityDataModel = std::make_unique<EntityDataModel>(std::move(entity));
    bui->setupUi(this);
    connectSlots();
    bui->tvEntity->setModel(mEntityDataModel.get());
    bui->tvEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    populateFilterCombo();
}

/*
void BaseEntityBrowserDlg::setEntityDataModel(BaseEntity* entity)
{
    if (mEntityDataModel){
        //delete mEntityDataModel;
        delete mBaseEntity;
    }
    mEntityDataModel = std::make_unique<EntityDataModel>(entity);
    mBaseEntity = entity;
}
*/

BaseEntityBrowserDlg::~BaseEntityBrowserDlg()
{
    delete bui;
}

void BaseEntityBrowserDlg::connectSlots()
{
    connect(bui->btnAdd, &QPushButton::clicked, this, &BaseEntityBrowserDlg::addBtnClicked);
    connect(bui->btnEdit, &QPushButton::clicked, this, &BaseEntityBrowserDlg::editBtnClicked);
    connect(bui->btnDelete, &QPushButton::clicked, this, &BaseEntityBrowserDlg::deleteBtnClicked);
    connect(bui->btnSearch, &QPushButton::clicked, this, &BaseEntityBrowserDlg::searchBtnClicked);
}

void BaseEntityBrowserDlg::setDialogTitle(const QString title)
{
    this->setWindowTitle(title);
}

void BaseEntityBrowserDlg::setMdiArea(QMdiArea* mdi)
{
   mMdiArea = mdi;
}
EntityDataModel& BaseEntityBrowserDlg::entityDataModel() const
{
    return *mEntityDataModel;
}

void BaseEntityBrowserDlg::addBtnClicked()
{
    addRecord();
}

void BaseEntityBrowserDlg::editBtnClicked()
{
    if (selectedRowId() < 0){
        showMessage("Select a record to edit.");
    }else{
        updateRecord();
    }
}

void BaseEntityBrowserDlg::deleteBtnClicked()
{
    if (selectedRowId() < 0){
        showMessage("Select a record to delete.");
    }else{
        if (QMessageBox::question(this, tr("Traffik"),
                                    tr("Are you sure you want to delete the record?"),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)

        deleteRecord();
    }
}

void BaseEntityBrowserDlg::searchBtnClicked()
{
    try{
        searchRecord();
    }
    catch(DatabaseException& de){
       showMessage(de.errorMessage());
    }
}

void BaseEntityBrowserDlg::searchRecord()
{
    if (bui->edtFilter->text().isEmpty()){
        entityDataModel().all();
    }else{
        auto data = bui->cbFilter->itemData(
                            bui->cbFilter->currentIndex()).value<QVariant>();
        std::string columnName = data.toString().toStdString();
        std::string item = bui->edtFilter->text().toStdString();
        auto searchItem = std::make_tuple(columnName, item);
        entityDataModel().searchByStr(searchItem);
    }
}

void BaseEntityBrowserDlg::deleteRecord()
{
   BaseEntity* entity = findSelectedEntity();
   entity->setDBAction(DBAction::dbaDELETE);

   try{
       entityDataModel().deleteEntity(*entity);
       removeSelectedRow();
   }
   catch (DatabaseException& de){
       showMessage(de.errorMessage());
    }
}

BaseEntity* BaseEntityBrowserDlg::findSelectedEntity()
{
   std::string searchName = selectedRowName().toStdString();
   BaseEntity* entity = entityDataModel().findEntityByName(searchName);
   return entity;
}

int BaseEntityBrowserDlg::selectedRowId() const
{
    return bui->tvEntity->currentIndex().row();
}

QString BaseEntityBrowserDlg::selectedRowName()
{

    QVariant col_name = bui->tvEntity->model()->data(
                            bui->tvEntity->model()->index(
                                    selectedRowId(), 0));
    return col_name.toString();
}

void BaseEntityBrowserDlg::removeSelectedRow()
{
    bui->tvEntity->model()->removeRow(selectedRowId());
}

void BaseEntityBrowserDlg::updateTableViewRecord(const std::vector<std::string> values)
{
    //std::vector<std::string> values = entity->tableViewValues();
    int col = 0;
    for (auto value: values){
        QVariant qvalue(QString::fromStdString(value));
        QModelIndex modelIndex = bui->tvEntity->model()->index(selectedRowId(), col);
        QAbstractItemModel* model = bui->tvEntity->model();
        model->setData(modelIndex, qvalue);
        ++col;
    }
}

void BaseEntityBrowserDlg::populateFilterCombo()
{
    //auto cIter = mBaseEntity->cBeginIter();
    //for(; cIter != mBaseEntity->cEndIter(); ++cIter){
    for (auto& fld : entityDataModel().getEntity().fields()){
        auto ptr(std::get<1>(fld).get());
        if (ptr->searchable())
            bui->cbFilter->addItem(stoq(ptr->fieldLabel()),
                               QVariant(stoq(ptr->dbColumnName())));
    }
}

void BaseEntityBrowserDlg::hideAddButton()
{
    bui->btnAdd->setVisible(false);
}
void BaseEntityBrowserDlg::hideEditButton()
{
    bui->btnEdit->setVisible(false);
}
void BaseEntityBrowserDlg::hideDeleteButton()
{
    bui->btnDelete->setVisible(false);
}

std::string BaseEntityBrowserDlg::typeID()
{
    return "BaseEntityBrowserDlg";
}
