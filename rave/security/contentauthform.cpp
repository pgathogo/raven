#include "contentauthform.h"
#include "ui_contentauthform.h"
#include "ui_baseentitydetaildlg.h"

#include "contentauth.h"

ContentAuthForm::ContentAuthForm(ContentAuth* cauth, QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::ContentAuthForm),
    mContentAuth{cauth}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    populateFormWidgets();

    connect(ui->cbContent, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ContentAuthForm::contentChanged);

    connect(ui->cbRole, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ContentAuthForm::roleChanged);
}

ContentAuthForm::~ContentAuthForm()
{
    delete ui;
}

std::string ContentAuthForm::windowTitle()
{
  return "Content Authorization Details";
}

void ContentAuthForm::populateEntityFields()
{
    contentChanged(ui->cbContent->currentIndex());
    roleChanged(ui->cbRole->currentIndex());
    auto set_bit = [](Qt::CheckState cs){ return (cs==Qt::Checked) ? 1 : 0; };
    mContentAuth->setCreateBit(set_bit(ui->cbCreate->checkState()));
    mContentAuth->setReadBit(set_bit(ui->cbRead->checkState()));
    mContentAuth->setUpdateBit(set_bit(ui->cbUpdate->checkState()));
    mContentAuth->setDeleteBit(set_bit(ui->cbDelete->checkState()));

    mContentAuth->setAccessBit(mContentAuth->access_bit_tostring());

}

void ContentAuthForm::populateFormWidgets()
{
    ui->cbContent->setModel(mContentAuth->content()->dataModel());
    ui->cbContent->setCurrentIndex(
                ui->cbContent->findText(
                    stoq(mContentAuth->content()->displayName())));

    ui->cbRole->setModel(mContentAuth->role()->dataModel());
    ui->cbRole->setCurrentIndex(
                ui->cbRole->findText(
                    stoq(mContentAuth->role()->displayName())));

    auto cs = [](int i){ return (i==0) ? Qt::Unchecked : Qt::Checked; };
    ui->cbCreate->setCheckState(cs(mContentAuth->createBit()->value()));
    ui->cbRead->setCheckState(cs(mContentAuth->readBit()->value()));
    ui->cbUpdate->setCheckState(cs(mContentAuth->updateBit()->value()));
    ui->cbDelete->setCheckState(cs(mContentAuth->deleteBit()->value()));


}

ActionResult ContentAuthForm::saveRecord()
{
    populateEntityFields();
    return mContentAuth->validate();
}

void ContentAuthForm::contentChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbContent->model());
    mContentAuth->setContent(std::get<1>(*(edm->vecBegin()+i))->id());
}

void ContentAuthForm::roleChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbRole->model());
    mContentAuth->setRole(std::get<1>(*(edm->vecBegin()+i))->id());
}
