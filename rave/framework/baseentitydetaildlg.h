#ifndef BASEENTITYDETAILDLG_H
#define BASEENTITYDETAILDLG_H

#include <QDialog>
#include <QKeyEvent>

#include "../utils/types.h"

//class EntityDataModel;
class BaseEntity;
class NotificationBar;

namespace Ui {
class BaseEntityDetailDlg;
}

class BaseEntityDetailDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BaseEntityDetailDlg(QDialog *parent = nullptr);
    ~BaseEntityDetailDlg();

    void connectSlots();
    [[nodiscard]] virtual ActionResult saveRecord()=0;

    //virtual void populateFields()=0;
    virtual void bindWidgets()=0;

    std::string title();
    virtual std::string windowTitle() = 0;

    void closeEvent(QCloseEvent* event);

private slots:
    void btnSaveClicked();
    void btnCloseClicked();
    void btnSaveNewClicked();

protected:
    Ui::BaseEntityDetailDlg* bui;
    void save(BaseEntity* entity);
    void setTitle(std::string title);

private:
   NotificationBar* mNoticeBar;
   bool mOkClose;
};

#endif // BASEENTITYDETAILDLG_H
