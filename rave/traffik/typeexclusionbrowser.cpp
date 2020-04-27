#include "typeexclusionbrowser.h"
#include "ui_typeexclusionbrowser.h"

#include "typeexclusion.h"
#include "typeexclusiondetails.h"

TypeExclusionBrowser::TypeExclusionBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, new TypeExclusion),
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
    addEntity<TypeExclusion, TypeExclusionDetails>();
}

void TypeExclusionBrowser::updateRecord()
{
   update<TypeExclusion, TypeExclusionDetails>();
}
