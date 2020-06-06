#ifndef VALUELISTDETAILDLG_H
#define VALUELISTDETAILDLG_H

#include <QDialog>
#include <memory>
#include "../framework/baseentitydetaildlg.h"

class BaseEntity;
class ValueList;

namespace Ui {
class ValueListDetailDlg;
}

class ValueListDetailDlg : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit ValueListDetailDlg(ValueList* entity,
                                QDialog *parent = nullptr);
    ~ValueListDetailDlg() override;

    ActionResult saveRecord() override;

    std::string windowTitle() override;

    void populateEntityFields() override;

    void populateFormWidgets() override;
private:
    Ui::ValueListDetailDlg *ui;
    ValueList* mEntity;

};

#endif // VALUELISTDETAILDLG_H
