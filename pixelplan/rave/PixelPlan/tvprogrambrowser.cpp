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

}

void TVProgramBrowser::searchRecord()
{

}

bool TVProgramBrowser::okay_to_delete(std::shared_ptr<BaseEntity> entity)
{

}
