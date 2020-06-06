#include "typeexclusionbrowser.h"
#include "ui_typeexclusionbrowser.h"

#include "typeexclusion.h"
#include "typeexclusiondetails.h"

TypeExclusionBrowser::TypeExclusionBrowser(QWidget *parent,
                                           TypeExclusion* tEx) :
    BaseEntityBrowserDlg(parent, new TypeExclusion),
    ui(new Ui::TypeExclusionBrowser),
    mTypeExcl{tEx},
    mTypeExD{}
{
    ui->setupUi(this);
    setDialogTitle("Type Exclusions");

}

TypeExclusionBrowser::~TypeExclusionBrowser()
{
    delete mTypeExcl;
    delete mTypeExD;
    delete ui;
}

void TypeExclusionBrowser::addRecord()
{
    //addEntity<TypeExclusion, TypeExclusionDetails>();
    mTypeExcl = new TypeExclusion();
    mTypeExD = new TypeExclusionDetails(mTypeExcl);
    if (mTypeExD->exec() > 0)
        entityDataModel()->createEntity(mTypeExcl);
}

void TypeExclusionBrowser::updateRecord()
{
   update<TypeExclusion, TypeExclusionDetails>();
}

std::string TypeExclusionBrowser::typeID()
{
    return "TypeExclusionBrowser";
}
