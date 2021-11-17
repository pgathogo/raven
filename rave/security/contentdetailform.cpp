#include "contentdetailform.h"
#include "ui_contentdetailform.h"
#include "../framework/ui_baseentitydetaildlg.h"

#include "content.h"

ContentDetailForm::ContentDetailForm(Content* content, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ContentDetailForm),
    mContent{ content }
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    populateFormWidgets();
}

ContentDetailForm::~ContentDetailForm()
{
    delete ui;
}

ActionResult ContentDetailForm::saveRecord()
{
    populateEntityFields();
    return mContent->validate();
}

std::string ContentDetailForm::windowTitle()
{
    return "Content Details";
}

void ContentDetailForm::populateEntityFields()
{
    mContent->contentDisplayName()->setValue(
                ui->edtDisplayName->text().toStdString());
}

void ContentDetailForm::populateFormWidgets()
{
    ui->edtName->setText(QString::fromStdString(mContent->name()->value()));
    ui->edtDisplayName->setText(QString::fromStdString(mContent->contentDisplayName()->value()));
}
