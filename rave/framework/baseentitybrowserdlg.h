#ifndef BASEENTITYBROWSERDLG_H
#define BASEENTITYBROWSERDLG_H

#include <QDialog>
#include <QMdiArea>

class BaseEntity;
class EntityDataModel;
class QTableView;

namespace Ui {
class BaseEntityBrowserDlg;
}

class BaseEntityBrowserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BaseEntityBrowserDlg(QWidget *parent = nullptr);
    BaseEntityBrowserDlg(QWidget* parent, BaseEntity* entity);
    ~BaseEntityBrowserDlg();

    void setDialogTitle(const QString title);
    void connectSlots();
    void setMdiArea(QMdiArea* mdi);

    int selectedRowId();
    QString selectedRowName();
    void updateTableViewRecord(BaseEntity* entity);

    virtual void addRecord()=0;
    virtual void updateRecord()=0;
    virtual void searchRecord()=0;

protected:
    EntityDataModel* entityDataModel() const;
    QMdiArea* mMdiArea;

public slots:
    void addBtnClicked();
    void editBtnClicked();
    void searchBtnClicked();

private:
    Ui::BaseEntityBrowserDlg* bui;
    BaseEntity* mBaseEntity;
    EntityDataModel* mEntityDataModel;
};

#endif // BASEENTITYBROWSERDLG_H
