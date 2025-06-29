#ifndef TVPROGRAMBROWSER_H
#define TVPROGRAMBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

namespace Ui {
    class TVProgramBrowser;
}

class TVProgramBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit TVProgramBrowser(QWidget *parent = nullptr);
    ~TVProgramBrowser();


    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;
    bool okay_to_delete(std::shared_ptr<BaseEntity> entity) override;

private:
    Ui::TVProgramBrowser *ui;
};

#endif // TVPROGRAMBROWSER_H
