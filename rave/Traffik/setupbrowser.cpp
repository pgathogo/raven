#include <QVBoxLayout>
#include "setupbrowser.h"
#include "ui_setupbrowser.h"

#include "clientgroupdlg.h"
#include "typeexclusionbrowser.h"
#include "voiceoverbrowser.h"
#include "salespersonbrowser.h"
#include "agentbrowser.h"
#include "timebandbrowser.h"
#include "../security/userbrowser.h"
#include "../security/rolebrowser.h"
#include "../security/contentbrowser.h"
#include "../security/contentauthbrowser.h"


#include "../framework/valuelistbrowser.h"

SetupBrowser::SetupBrowser(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SetupBrowser)
    ,m_mdi_area{nullptr}
    ,m_client_group_tab{nullptr}
    ,m_gender_tab{nullptr}
{
    ui->setupUi(this);

    m_client_group_tab = new TabWidget(new ClientGroupDlg(this));
    m_gender_tab = new TabWidget(new ValueListBrowser("gender"));
    m_type_exclusion_tab = new TabWidget(new TypeExclusionBrowser());
    m_voice_over_tab = new TabWidget(new VoiceOverBrowser());
    m_sales_person_tab = new TabWidget(new SalesPersonBrowser());
    m_agents_tab = new TabWidget(new AgentBrowser());
    m_timeband_tab = new TabWidget(new TimeBandBrowser());
    m_users_tab = new TabWidget(new UserBrowser());
    m_role_tab = new TabWidget(new RoleBrowser());
    m_content_tab = new TabWidget(new ContentBrowser());
    m_content_auth_tab = new TabWidget(new ContentAuthBrowser());

    ui->tabMain->addTab(m_client_group_tab, "Client Group");
    ui->tabMain->addTab(m_gender_tab, "Gender");
    ui->tabMain->addTab(m_type_exclusion_tab, "Type Exclusions");
    ui->tabMain->addTab(m_voice_over_tab, "Voice Over");
    ui->tabMain->addTab(m_sales_person_tab, "Sales Persons");
    ui->tabMain->addTab(m_agents_tab, "Agents");
    ui->tabMain->addTab(m_timeband_tab, "Time Bands");
    ui->tabMain->addTab(m_users_tab, "System Users");
    ui->tabMain->addTab(m_role_tab, "System Roles");
    ui->tabMain->addTab(m_content_tab,"Contents");
    ui->tabMain->addTab(m_content_auth_tab, "Content Authorization");

    ui->tabMain->setCurrentIndex(0);

    setFixedSize(1035, 500);

}

SetupBrowser::~SetupBrowser()
{
    //delete m_client_group_browser;
    delete ui;
}

void SetupBrowser::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;
}

TabWidget::TabWidget(BaseEntityBrowserDlg* browser)
    :m_vert_layout{nullptr}
    ,m_browser{browser}
{
    m_vert_layout = new QVBoxLayout();
    setLayout(m_vert_layout);
    m_vert_layout->addWidget(browser);
}

TabWidget::~TabWidget()
{
    delete m_browser;
    delete m_vert_layout;
}

void TabWidget::add_widget(QWidget* widget)
{
    m_vert_layout->addWidget(widget);
}

QVBoxLayout* TabWidget::get_layout()
{
    return m_vert_layout;
}
