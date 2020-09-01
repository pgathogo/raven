#include <algorithm>
#include "contentform.h"
#include "ui_contentform.h"
#include "ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entityregister.h"
#include "../framework/entitydatamodel.h"
#include "contentdetailform.h"

#include "content.h"

ContentForm::ContentForm(Content* content, EntityDataModel& edm,
                         QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ContentForm),
    mContent{content},
    edmContent{edm}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    connect(ui->btnRegister, &QPushButton::clicked, this, &ContentForm::registerContent);
    connect(ui->btnRemove, &QPushButton::clicked, this, &ContentForm::deregisterContent);

    ui->lvRegContent->setModel(&edmContent);

    populateFormWidgets();
}

ContentForm::~ContentForm()
{
    delete ui;
}

ActionResult ContentForm::saveRecord()
{
    compare_temp_list();
    return std::make_tuple(ActionResultType::arSUCCESS,"");
}

std::string ContentForm::windowTitle()
{
    return "Application Contents";
}

void ContentForm::populateEntityFields()
{
}

void ContentForm::populateFormWidgets()
{
    populateAppContent();
    make_temp_list();
}

void ContentForm::populateAppContent()
{
    auto ibegin = edmContent.modelEntities().begin();
    auto iend = edmContent.modelEntities().end();

    for (auto& [name, func] : Factory<BaseEntity, int>::_data()){
        auto nn = name;
        // Only show content that has not yet been registered
        if ( std::find_if(ibegin, iend, [&nn](EntityRecord const& er){
                         return ( nn==std::get<0>(er) );  } ) == iend ){

            QListWidgetItem* wItem = new QListWidgetItem();
            QString entityTableName = stoq(func(0)->tableName());
            wItem->setData(Qt::UserRole, entityTableName);
            wItem->setText(stoq(name));
            ui->lvAppContent->addItem(wItem);
            //ui->lvAppContent->addItem(QString::fromStdString(name));
        }
    }
}

void ContentForm::make_temp_list()
{
    for (auto& [name, entity] : edmContent.modelEntities()){
        temp_list.push_back(name);
    }
}

void ContentForm::compare_temp_list()
{
    // If an entity is marked for creation but then it is found in the
    // temp list, no need to create, it is assumed it already exists
    // in the database.
    for (auto& [name, entity] : edmContent.modelEntities()){
        auto nn = name;
        if (std::find_if(temp_list.begin(), temp_list.end(),
                         [&nn](std::string key){
                         return ( nn == key );}) != temp_list.end() )
        {
            if (entity->dbAction() == DBAction::dbaCREATE)
                entity->setDBAction(DBAction::dbaNONE);
        }
    }

}

void ContentForm::registerContent()
{
    auto content = std::make_unique<Content>();

    if (ui->lvAppContent->selectedItems().size() > 0){

        content->name()->setValue(
                    ui->lvAppContent->currentItem()->text().toStdString());

        QVariant data = ui->lvAppContent->currentItem()->data(Qt::UserRole);
        content->contentTableName()->setValue(
                    data.toString().toStdString() );

        auto cd = std::make_unique<ContentDetailForm>(content.get());
        if (cd->exec() > 0){
            content->setDBAction(DBAction::dbaCREATE);
            edmContent.cacheEntity(std::move(content));
            ui->lvAppContent->takeItem(ui->lvAppContent->currentRow());
        }
    }
}

void ContentForm::deregisterContent()
{
    if (ui->lvRegContent->currentIndex().row() > 0){

        auto modIdx = ui->lvRegContent->currentIndex();
        std::string sel_item = modIdx.data(Qt::DisplayRole).toString().toStdString();
        BaseEntity* entity = edmContent.findEntityByName(sel_item);
        entity->setDBAction(DBAction::dbaDELETE);
        edmContent.removeRow(ui->lvRegContent->currentIndex().row());
        ui->lvAppContent->addItem(QString::fromStdString(sel_item));
    }

}
