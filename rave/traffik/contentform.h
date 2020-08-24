#ifndef CONTENTFORM_H
#define CONTENTFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class Content;

namespace Ui {
class ContentForm;
}

class ContentForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ContentForm(Content* content, EntityDataModel& edm,
                         QDialog* parent = nullptr);
    ~ContentForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;

    void populateAppContent();
    bool findContent(std::string cont_name);
    void make_temp_list();
    void compare_temp_list();

private slots:
    void registerContent();
    void deregisterContent();

private:
    Ui::ContentForm *ui;
    Content* mContent;
    EntityDataModel& edmContent;
    std::vector<std::string> temp_list;
};

#endif // CONTENTFORM_H
