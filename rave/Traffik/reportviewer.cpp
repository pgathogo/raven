#include <QMdiArea>
#include <QJsonDocument>
#include <QWebEngineView>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>

#include "reportviewer.h"


ReportViewer::ReportViewer(QJsonObject data, QString report_file, QMdiArea* mdi_area, bool show_dev_tools)
    :m_data{data}
    ,m_report_file{report_file}
    ,m_show_dev_tools{show_dev_tools}
{
    setupui();

    mdi_area->addSubWindow(this);

    render_report();
}

void ReportViewer::setupui()
{
    m_main_layout = new QVBoxLayout(this);

    // Make toolbar
    QHBoxLayout* hl_toolbar = new QHBoxLayout();

    QPushButton* btn_save_pdf = new QPushButton("Save as PDF...");
    connect(btn_save_pdf, &QPushButton::clicked, this, &ReportViewer::on_save_pdf);

    hl_toolbar->addWidget(btn_save_pdf);
    hl_toolbar->addStretch(1);

    m_main_layout->addLayout(hl_toolbar);

    setMinimumSize(1000, 525);

}

void ReportViewer::on_save_pdf()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Save File As"),
        "",
        tr("PDF Files (*.pdf)"));

    if (filename.isEmpty())
        return;

    m_web_view->page()->printToPdf(filename);
}

void ReportViewer::on_pdf_print_finished(const QString& filepath, bool success)
{
    if (success) {
        qDebug() << "PDF successfully save to: " << filepath;
    } else {
        qWarning() << "Failed to save PDF to: " << filepath;
    }

}

void ReportViewer::render_report()
{
    QJsonDocument doc(m_data);
    QString json_string = doc.toJson(QJsonDocument::Compact);
    QString escaped_json = json_string;

    escaped_json.replace("\\", "\\\\");
    escaped_json.replace("'", "\\");

    m_web_view = new QWebEngineView(this);
    m_web_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_web_view, &QWebEngineView::loadFinished, [this, escaped_json](bool ok){
        if (ok) {
            QString js_call = QString("render_report('%1');").arg(escaped_json);
            m_web_view->page()->runJavaScript(js_call);
        }
    });

    connect(m_web_view->page(), &QWebEnginePage::pdfPrintingFinished, this, &ReportViewer::on_pdf_print_finished);

    m_main_layout->addWidget(m_web_view);

    QString report_file = QString("qrc:/reports/build/html/%1").arg(m_report_file);
    m_web_view->load(QUrl(report_file));

    // Devtools
    if (m_show_dev_tools) {
        QWebEngineView* devtools = new QWebEngineView();
        m_web_view->page()->setDevToolsPage(devtools->page());
        devtools->resize(1200, 800);
        devtools->show();
    }

}

