#ifndef BASEENTITYBROWSERDLG_H
#define BASEENTITYBROWSERDLG_H

#include <QDialog>
#include <QMdiArea>

class BaseEntity;
class EntityDataModel;

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

protected:
    EntityDataModel* entityDataModel() const;
    QMdiArea* mMdiArea;

public slots:
    virtual void AddRecord()=0;
    virtual void searchRecord();

private:
    Ui::BaseEntityBrowserDlg *ui;
    BaseEntity* mBaseEntity;
    EntityDataModel* mEntityDataModel;
};

#endif // BASEENTITYBROWSERDLG_H
