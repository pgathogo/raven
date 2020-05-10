#ifndef PLAINFORM_H
#define PLAINFORM_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

class DayPartGrid;
class ManyToManyBrowser;
class EntityDataModel;

namespace Ui {
class PlainForm;
}

class PlainForm : public QDialog
{
    Q_OBJECT

public:
    explicit PlainForm(QDialog *parent = nullptr);
    ~PlainForm();
    void fillGrid();

public slots:
    void onShow();
    void onClose();

private:
    Ui::PlainForm *ui;
    DayPartGrid* dpg;
    ManyToManyBrowser* mtom;
    EntityDataModel* edm;
};

#endif // PLAINFORM_H
