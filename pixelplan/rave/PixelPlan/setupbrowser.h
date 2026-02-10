#pragma once

#include <QDialog>
#include <memory>
#include "../../../rave/framework/baseentitybrowserdlg.h"


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
    TabWidget* m_type_exclusion_tab;
    TabWidget* m_voice_over_tab;
    TabWidget* m_sales_person_tab;
    TabWidget* m_agents_tab;
    TabWidget* m_timeband_tab;
    TabWidget* m_users_tab;
    TabWidget* m_role_tab;
    TabWidget* m_content_tab;
    TabWidget* m_content_auth_tab;
    TabWidget* m_booking_void_reason_tab;
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

