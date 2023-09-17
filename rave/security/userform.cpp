#include "userform.h"
#include "ui_userform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/entitydatamodel.h"
#include "../utils/tools.h"

#include "user.h"
#include "role.h"

UserForm::UserForm(SECURITY::User* user, QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::UserForm),
    mUser{user}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    connect(ui->cbResetPassword, &QCheckBox::stateChanged, this, &UserForm::updateValidity);

    populateFormWidgets();

    ui->edtExpireDate->setEnabled(false);

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
    mUser->set_role_name(ui->edtUserName->text().toStdString());
    mUser->set_rol_password(ui->edtPassword->text().toStdString());

    bool reset_password = [&](){ return (ui->cbResetPassword->checkState()==Qt::Checked) ? true : false ; }();

    mUser->set_reset_password(reset_password);

//    if (reset_password)
//        mUser->set_valid_until(QDate::currentDate().addDays(-5).toString("yyyy-MM-dd").toStdString());
//    else
    mUser->set_valid_until("infinity");
}

void UserForm::populateFormWidgets()
{
    ui->edtUserName->setText(stoq(mUser->role_name()->value()));
    ui->edtUserName->setEnabled(mUser->role_name()->value().empty());

    if (!mUser->valid_until()->value().empty() &&
            mUser->valid_until()->value() != "infinity"){

        auto [y, m, d] = ymd(mUser->valid_until()->value());

        QDate qdate = QDate(y, m, d);

        ui->edtExpireDate->setDate(qdate);
        ui->edtExpireDate->setEnabled(true);
    }

    if (mUser->valid_until()->value() == "infinity" ||
            mUser->valid_until()->value().empty()){
        //ui->cbResetPassword->setCheckState(Qt::Checked);
        //ui->edtExpireDate->setDate(QDate(QDate::currentDate().addYears(5)));
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

//    if (mUser->password()->value() != mUser->confirmPassword()->value()){
//        ar = std::make_tuple(ActionResultType::arERROR,
//                             "Password and Confirmation password not same!");
//        return ar;
//    }

    // check length of password
    if (mUser->rol_password()->value().length() < min_password_length){
        ar = std::make_tuple(ActionResultType::arERROR,
                             "To few characters in password!");
        return ar;
    }

//    if (mUser->passwordExpire()->value()){
//        // check if expiry is > than current date
//    }

    // For new user, check if there is an existing user with the same name
    if (ui->edtUserName->isEnabled()){

        try{
            EntityDataModel edm = EntityDataModel(std::make_unique<SECURITY::User>());
            auto user_filter = std::make_tuple("lower("+mUser->role_name()->dbColumnName()+")",
                               "=",
                               mUser->role_name()->value());
                               //mUser->role_name()->dbValueFormatter());
            std::string filter = edm.prepareFilter(user_filter);
            edm.search(filter);

            if (edm.entitiesCount() > 0){
                ar = std::make_tuple(ActionResultType::arERROR,
                         "The user name is already in use!");
                return ar;
            }
        } catch(DatabaseException& de) {
            std::cout << de.errorMessage() << '\n';
            showMessage(de.errorMessage());
        }


    }

    return ar;
}

void UserForm::updateValidity(int state)
{
    /*
    if (state == Qt::Checked)
        ui->edtExpireDate->setEnabled(false);
    else
        ui->edtExpireDate->setEnabled(true);
   */
}
