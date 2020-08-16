#ifndef USERFORM_H
#define USERFORM_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

class User;

namespace Ui {
class UserForm;
}

class UserForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    const size_t min_password_length = 4;

    explicit UserForm(User* user, QDialog *parent = nullptr);
    ~UserForm() override;

    std::string windowTitle() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;
    ActionResult saveRecord() override;

private slots:
    void updateValidity(int);

private:
    Ui::UserForm *ui;
    User* mUser;
};

#endif // USERFORM_H
