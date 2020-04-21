#ifndef VALUELISTDETAILDLG_H
#define VALUELISTDETAILDLG_H

#include <QDialog>
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
    explicit ValueListDetailDlg(ValueList* entity, QDialog *parent = nullptr);
    ~ValueListDetailDlg() override;

    ActionResult saveRecord() override;
    void bindWidgets() override;

    std::string title() override;

    void setEntity(BaseEntity* entity);

private:
    Ui::ValueListDetailDlg *ui;
    ValueList* mEntity;

};

#endif // VALUELISTDETAILDLG_H
