#include "typeexclusionbrowser.h"
#include "ui_typeexclusionbrowser.h"

#include "../framework/typeexclusion.h"
#include "typeexclusiondetails.h"

TypeExclusionBrowser::TypeExclusionBrowser(QWidget *parent):
    BaseEntityBrowserDlg(parent,
                         std::make_unique<TRAFFIK::TypeExclusion>()),
    ui(new Ui::TypeExclusionBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Type Exclusions");

}

TypeExclusionBrowser::~TypeExclusionBrowser()
{
    delete ui;
}

void TypeExclusionBrowser::addRecord()
{
    auto tex = std::make_shared<TRAFFIK::TypeExclusion>();

    std::unique_ptr<TypeExclusionDetails> texForm =
            std::make_unique<TypeExclusionDetails>(tex.get());

    if (texForm->exec() > 0)
        entityDataModel().createEntity(std::move(tex));
}

void TypeExclusionBrowser::updateRecord()
{
   update<TRAFFIK::TypeExclusion, TypeExclusionDetails>();
}

std::string TypeExclusionBrowser::typeID()
{
    return "TypeExclusionBrowser";
}
