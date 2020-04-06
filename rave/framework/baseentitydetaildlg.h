#ifndef BASEENTITYDETAILDLG_H
#define BASEENTITYDETAILDLG_H

#include <QDialog>

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
    virtual void saveRecord()=0;
    virtual void populateFields()=0;

private slots:
    void btnSaveClicked();
    void btnCloseClicked();

protected:
    Ui::BaseEntityDetailDlg* bui;
    void save(BaseEntity* entity);
    void setTitle(std::string title);
};

#endif // BASEENTITYDETAILDLG_H
