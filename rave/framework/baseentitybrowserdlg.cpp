#include <QMessageBox>

#include "baseentitybrowserdlg.h"

#include "baseentity.h"
#include "entitydatamodel.h"

#include "../utils/tools.h"
#include "../framework/ravenexception.h"
#include "../framework/letterfilterwidget.h"
#include "../security/authentication.h"

BaseEntityBrowserDlg::BaseEntityBrowserDlg( QWidget *parent)
    :QDialog(parent)
    ,mMdiArea{nullptr}
    ,bui(new Ui::BaseEntityBrowserDlg)
    ,mEntityDataModel{nullptr}
    ,m_letter_filter_widget{nullptr}
{
    bui->setupUi(this);
    connectSlots();
}

BaseEntityBrowserDlg::BaseEntityBrowserDlg(QWidget* parent,
                                           std::unique_ptr<BaseEntity> entity
                                           )
        :QDialog(parent)
        ,mMdiArea{nullptr}
        ,bui(new Ui::BaseEntityBrowserDlg)
        ,mEntityDataModel{nullptr}
        ,m_letter_filter_widget{nullptr}
{

    bui->setupUi(this);
    connectSlots();

    //auto access_bit = Authentication::mAccessMap[entity->className()];
    //setAccess(access_bit);

    mEntityDataModel = std::make_unique<EntityDataModel>(std::move(entity));

    bui->tvEntity->setModel(mEntityDataModel.get());
    bui->tvEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    populateFilterCombo();

}

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

void BaseEntityBrowserDlg::show_letter_filter()
{
    m_letter_filter_widget = std::make_unique<LetterFilterWidget>(this);
    bui->vlGrid->addWidget(m_letter_filter_widget.get());

    connect(m_letter_filter_widget->get_tabwidget(), &QTabWidget::currentChanged, this, BaseEntityBrowserDlg::filter_by_letter);
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
    entityDataModel().clearEntities();

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

void BaseEntityBrowserDlg::filter_by_letter(int index)
{
    entityDataModel().clearEntities();

    QString tab_text = m_letter_filter_widget->get_tabwidget()->tabText(index);

    auto data = bui->cbFilter->itemData(
                        bui->cbFilter->currentIndex()).value<QVariant>();
    std::string columnName = data.toString().toStdString();
    auto searchItem = std::make_tuple(columnName, tab_text.toStdString());
    entityDataModel().starts_with(searchItem);

}

bool BaseEntityBrowserDlg::okay_to_delete(BaseEntity* /*entity*/)
{
    return true;
}

void BaseEntityBrowserDlg::deleteRecord()
{
  if (selectedRowId() < 0)
      return;

  BaseEntity* entity = findSelectedEntity();

  if (okay_to_delete(entity) ){

       entity->setDBAction(DBAction::dbaDELETE);

       try{
           entityDataModel().deleteEntity(*entity);
           removeSelectedRow();
       }
       catch (DatabaseException& de){
           showMessage(de.errorMessage());
        }
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
    if (bui->tvEntity->selectionModel()->selectedRows().size() > 0)
        return bui->tvEntity->selectionModel()->selectedRows()[0].row();

    return -1;
}

QString BaseEntityBrowserDlg::selectedRowName()
{

    auto sel_row_id = selectedRowId();
    if (sel_row_id == -1)
        return QString("");

    QVariant col_name = bui->tvEntity->model()->data(
                            bui->tvEntity->model()->index(
                                    sel_row_id, 0));
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

void BaseEntityBrowserDlg::show_delete_button()
{
   bui->btnDelete->setVisible(true);
}

std::string BaseEntityBrowserDlg::typeID()
{
    return "BaseEntityBrowserDlg";
}

void BaseEntityBrowserDlg::setAccess(std::string access_bit)
{
    if (access_bit.empty()) return;

    std::string create{access_bit.at(0)};
    std::string read{access_bit.at(1)};
    std::string update{access_bit.at(2)};
    std::string del{access_bit.at(3)};
    bui->btnAdd->setEnabled(std::atoi(create.c_str()));
    bui->btnSearch->setEnabled(std::atoi(read.c_str()));
    bui->btnEdit->setEnabled(std::atoi(update.c_str()));
    bui->btnDelete->setEnabled(std::atoi(del.c_str()));
}
