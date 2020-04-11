#ifndef BASEENTITYDETAILDLG_H
#define BASEENTITYDETAILDLG_H

#include <QDialog>
#include <QKeyEvent>

#include "../utils/types.h"

//class EntityDataModel;
class BaseEntity;

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
    [[nodiscard]] virtual ErrorMessage saveRecord()=0;
    virtual void populateFields()=0;
    void closeEvent(QCloseEvent* event);

private slots:
    void btnSaveClicked();
    void btnCloseClicked();

protected:
    Ui::BaseEntityDetailDlg* bui;
    void save(BaseEntity* entity);
    void setTitle(std::string title);

private:
   bool mOkClose;
};

#endif // BASEENTITYDETAILDLG_H
