#pragma once

#include <QDialog>
#include <memory>
#include "../framework/baseentitybrowserdlg.h"


class QMdiArea;
class QVBoxLayout;
class ClientGroupDlg;
class TabWidget;

namespace Ui {
class SetupBrowser;
}

class SetupBrowser : public QDialog
{
    Q_OBJECT

public:
    explicit SetupBrowser(QWidget *parent = nullptr);
    ~SetupBrowser();

    void setMdiArea(QMdiArea*);

private:
    Ui::SetupBrowser *ui;
    QMdiArea* m_mdi_area;

    TabWidget* m_client_group_tab;
    TabWidget* m_gender_tab;
};

class TabWidget : public QWidget
{
    public:
        explicit TabWidget(BaseEntityBrowserDlg* browser);
        ~TabWidget();

        void add_widget(QWidget*);

        QVBoxLayout* get_layout();

    private:
        QVBoxLayout* m_vert_layout;
        BaseEntityBrowserDlg* m_browser;
};

