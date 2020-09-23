#ifndef BREAKLAYOUTBROWSER_H
#define BREAKLAYOUTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

namespace Ui {
class BreakLayoutBrowser;
}

class BreakLayoutForm;

class BreakLayoutBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit BreakLayoutBrowser(QWidget* parent = nullptr);
    ~BreakLayoutBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;
    void saveBreakLayoutLines(const BreakLayoutForm& blf, int headerId);

    void addCreateBreakButton();

private slots:
    void openCreateBreaks();

private:
    Ui::BreakLayoutBrowser *ui;
};

#endif // BREAKLAYOUTBROWSER_H
