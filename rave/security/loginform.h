#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "accesscontroller.h"

class NotificationBar;
class Authentication;

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(Authentication* auth, QWidget* parent = nullptr);
    ~LoginForm();

    void closeEvent(QCloseEvent* event);
    bool validNamePass();

private slots:
    void signIn();
    void cancel();

private:
    Ui::LoginForm* ui;
    Authentication* mAuth;
    NotificationBar* mNoticeBar;
    bool mOkClose;
};

#endif // LOGINFORM_H
