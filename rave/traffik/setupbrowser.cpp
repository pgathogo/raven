#include <QVBoxLayout>
#include "setupbrowser.h"
#include "ui_setupbrowser.h"

#include "clientgroupdlg.h"
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

    ui->tabMain->addTab(m_client_group_tab, "Client Group");
    ui->tabMain->addTab(m_gender_tab, "Gender");

    ui->tabMain->setCurrentIndex(0);
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
