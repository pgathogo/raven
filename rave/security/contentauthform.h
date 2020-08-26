#ifndef CONTENTAUTHFORM_H
#define CONTENTAUTHFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class ContentAuth;

namespace Ui {
class ContentAuthForm;
}

class ContentAuthForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ContentAuthForm(ContentAuth*  cauth, QDialog* parent = nullptr);
    ~ContentAuthForm() override;

    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    ActionResult saveRecord() override;
    void setContentValue();
    void setRoleValue();

private slots:
    void contentChanged(int);
    void roleChanged(int);

private:
    Ui::ContentAuthForm *ui;
    ContentAuth* mContentAuth;
};

#endif // CONTENTAUTHFORM_H
