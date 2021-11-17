#include "breaklayoutbrowser.h"
#include "ui_breaklayoutbrowser.h"

#include "breaklayout.h"
#include "breaklayoutline.h"
#include "breaklayoutform.h"

#include "../utils/tools.h"
#include "../framework/ravenexception.h"

BreakLayoutBrowser::BreakLayoutBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_unique<BreakLayout>()),
    ui(new Ui::BreakLayoutBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Breaks Layout");

    addCreateBreakButton();
}

BreakLayoutBrowser::~BreakLayoutBrowser()
{
    delete ui;
}

void BreakLayoutBrowser::addRecord()
{
    auto bl = std::make_unique<BreakLayout>();
    auto blForm = std::make_unique<BreakLayoutForm>(bl.get());

    if (blForm->exec() > 0){
        qDebug() << "WEEK DAYS: "<< stoq(bl->weekDays()->value());
        try{
            int id = entityDataModel().createEntity(std::move(bl));
            saveBreakLayoutLines(*blForm, id);
        } catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void BreakLayoutBrowser::updateRecord()
{
    std::string searchName = selectedRowName().toStdString();
    if (!searchName.empty()){
        BaseEntity* be = entityDataModel().findEntityByName(searchName);
        auto breakLayout = dynamic_cast<BreakLayout*>(be);
        auto blForm = std::make_unique<BreakLayoutForm>(breakLayout);
        if (blForm->exec() > 0){
        }
    }
}

void BreakLayoutBrowser::deleteRecord()
{
    BaseEntity* entity = findSelectedEntity();
    entity->setDBAction(DBAction::dbaDELETE);

    try{
        // delete BreakLayoutLines
        BreakLayoutLine bll;
        std::string delStr = "Delete from "+bll.tableName()+" Where break_layout_id ="+std::to_string(entity->id());
        EntityDataModel edm;
        edm.executeRawSQL(delStr);

        // delete BreakLayout
        entityDataModel().deleteEntity(*entity);
        removeSelectedRow();

    } catch (DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

void BreakLayoutBrowser::saveBreakLayoutLines(const BreakLayoutForm& blf, int headerId)
{
    auto edm = std::make_unique<EntityDataModel>();

    auto& breakLines = blf.breakLines();

    for(auto& breakLine : breakLines){
        BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(std::get<1>(breakLine).get());
        bll->setBreakLayout(headerId);
        edm->createEntityDB(*bll);
    }

}

void BreakLayoutBrowser::addCreateBreakButton()
{
    QPushButton* btnCreateBreaks = new QPushButton("Create Schedule Breaks");
    btnCreateBreaks->setObjectName("btnCreateBreaks");
    connect(btnCreateBreaks, &QPushButton::clicked, this, &BreakLayoutBrowser::openCreateBreaks);
    bui->hlExtra->addWidget(btnCreateBreaks);
}

void BreakLayoutBrowser::openCreateBreaks()
{
    if (selectedRowId() < 0){
        showMessage("Please select a Break Layout.");
    } else {
    }

}
