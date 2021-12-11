#include "userform.h"
#include "ui_userform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"
#include "user.h"
#include "role.h"
#include "../utils/tools.h"

UserForm::UserForm(User* user, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::UserForm),
    mUser{user}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    connect(ui->cbExpiry, &QCheckBox::stateChanged, this, &UserForm::updateValidity);

    populateFormWidgets();

}

UserForm::~UserForm()
{
    delete ui;
}

std::string UserForm::windowTitle()
{
    return "User Details";
}

void UserForm::populateEntityFields()
{
    mUser->setUserName(ui->edtUserName->text().toStdString());
    mUser->setPassword(ui->edtPassword->text().toStdString());
    mUser->setPassword(ui->edtPassword->text().toStdString());
    mUser->setConfirmPassword(ui->edtConfirmPassword->text().toStdString());
    mUser->setValidUntil(ui->edtExpireDate->date().toString("yyyy-MM-dd").toStdString());
    bool canExpire = [&](){ return (ui->cbExpiry->checkState()==Qt::Checked) ? true : false ; }();
    mUser->setPasswordExpire(canExpire);
}

void UserForm::populateFormWidgets()
{
    ui->edtUserName->setText(stoq(mUser->userName()->value()));
    ui->edtUserName->setEnabled(mUser->userName()->value().empty());

    if (!mUser->validUntil()->value().empty() &&
            mUser->validUntil()->value() != "infinity"){

        auto [y, m, d] = ymd(mUser->validUntil()->value());

        QDate qdate = QDate(y, m, d);

        ui->edtExpireDate->setDate(qdate);
        ui->edtExpireDate->setEnabled(true);
    }

    if (mUser->validUntil()->value() == "infinity" ||
            mUser->validUntil()->value().empty()){
        ui->cbExpiry->setCheckState(Qt::Checked);
    }
}


ActionResult UserForm::saveRecord()
{
    populateEntityFields();

    ActionResult ar = std::make_tuple(ActionResultType::arSUCCESS,"");

    ar = mUser->validate();
    auto [result_type, msg ] = ar;
    if (result_type != ActionResultType::arSUCCESS)
            return ar;

    if (mUser->password()->value() != mUser->confirmPassword()->value()){
        ar = std::make_tuple(ActionResultType::arERROR,
                             "Password and Confirmation password not same!");
        return ar;
    }

    // check length of password
    if (mUser->password()->value().length() < min_password_length){
        ar = std::make_tuple(ActionResultType::arERROR,
                             "To few characters in password!");
        return ar;
    }

    if (mUser->passwordExpire()->value()){
        // check if expiry is > than current date
    }

    // For new user, check if there is an existing user with the same name
    if (ui->edtUserName->isEnabled()){
        EntityDataModel edm = EntityDataModel(std::make_unique<User>());
        auto user_filter = std::make_tuple("lower("+mUser->userName()->dbColumnName()+")",
                                           "=",
                                           mUser->userName()->value());
        std::string filter = edm.prepareFilter(user_filter);
        edm.search(filter);

        if (edm.entitiesCount() > 0){
            ar = std::make_tuple(ActionResultType::arERROR,
                                 "The user name is already in use!");
            return ar;
        }
    }

    return ar;
}

void UserForm::updateValidity(int state)
{
    if (state == Qt::Checked)
        ui->edtExpireDate->setEnabled(false);
    else
        ui->edtExpireDate->setEnabled(true);
}
