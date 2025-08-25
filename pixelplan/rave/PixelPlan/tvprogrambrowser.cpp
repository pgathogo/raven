#include <QDateTime>

#include "tvprogrambrowser.h"
#include "ui_tvprogrambrowser.h"
#include "../../../rave/framework/ui_baseentitybrowserdlg.h"

#include "tvprogram.h"
#include "tvprogramform.h"

TVProgramBrowser::TVProgramBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_unique<PIXELPLAN::TVProgram>()),
    ui(new Ui::TVProgramBrowser)
{
    ui->setupUi(this);

    show_letter_filter();

    setDialogTitle("TV Programs");

    setMinimumSize(860, 450);
}

TVProgramBrowser::~TVProgramBrowser()
{
    delete ui;
}

void TVProgramBrowser::addRecord()
{
    std::shared_ptr<PIXELPLAN::TVProgram> tvprog = std::make_shared<PIXELPLAN::TVProgram>();

    std::unique_ptr<TVProgramForm> tvform = std::make_unique<TVProgramForm>(tvprog);
    if (tvform->exec() > 0) {
        tvprog->set_created_at(QDateTime().currentDateTime());
        entityDataModel().createEntity(std::move(tvprog));
    }

}

void TVProgramBrowser::updateRecord()
{
    std::string search_name = selectedRowName().toStdString();
    if (search_name.empty())
        return;

    std::shared_ptr<BaseEntity> be = entityDataModel().findEntityByName(search_name);
    std::shared_ptr<PIXELPLAN::TVProgram> tvprogram = std::dynamic_pointer_cast<PIXELPLAN::TVProgram>(be);

    auto tvprog_form = std::make_unique<TVProgramForm>(tvprogram);

    if (tvprog_form->exec() == 0)
        return;

    try{
        updateTableViewRecord(tvprogram->tableViewValues());

        // We don't want to updated this field
        tvprogram->created_at()->setReadOnly(true);

        entityDataModel().updateEntity(*tvprogram);
        entityDataModel().all();
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }


}

