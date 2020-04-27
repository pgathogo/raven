#ifndef PLAINFORM_H
#define PLAINFORM_H

#include <QDialog>
#include "../framework/baseentitydetaildlg.h"

class DayPartGrid;
class ManyToManyBrowser;

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

private:
    Ui::PlainForm *ui;
    DayPartGrid* dpg;
    ManyToManyBrowser* mtom;
};

#endif // PLAINFORM_H
