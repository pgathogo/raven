#ifndef VALUELISTBROWSER_H
#define VALUELISTBROWSER_H

#include <QDialog>
#include "baseentitybrowserdlg.h"
#include "valuelist.h"

class EntityDataModel;
class ValueListDetailDlg;
class Gender;


namespace Ui {
class ValueListBrowser;
}

class ValueListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    //explicit ValueListBrowser(QWidget *parent = nullptr);
    ValueListBrowser(const std::string& vltype, QWidget* parent = nullptr);

    ~ValueListBrowser() override;

    void addRecord() override;
    void updateRecord() override;

  static ValueList*
  createValueList(const std::string& vlType)
  {
      if (vlType =="gender")
          return new Gender();

      return nullptr;
  }

private:
    Ui::ValueListBrowser *ui;
    ValueListDetailDlg* vdd;
    std::string vtype;

};

#endif // VALUELISTBROWSER_H
