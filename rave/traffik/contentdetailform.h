#ifndef CONTENTDETAILFORM_H
#define CONTENTDETAILFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"


class Content;

namespace Ui {
class ContentDetailForm;
}

class ContentDetailForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ContentDetailForm(Content* content, QDialog* parent = nullptr);
    ~ContentDetailForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
private:
    Ui::ContentDetailForm *ui;
    Content* mContent;
};

#endif // CONTENTDETAILFORM_H
