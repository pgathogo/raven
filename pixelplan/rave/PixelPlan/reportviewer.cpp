#include <QMdiArea>
#include <QJsonDocument>
#include <QWebEngineView>
#include <QUrl>
#include <QVBoxLayout>

#include "reportviewer.h"


ReportViewer::ReportViewer(QJsonObject data, QString report_file, QMdiArea* mdi_area)
    :m_data{data}
    ,m_report_file{report_file}
{
    setupui();
    mdi_area->addSubWindow(this);
    render_report();
}

void ReportViewer::setupui()
{
    setMinimumSize(1000, 525);

}

void ReportViewer::render_report()
{
    QJsonDocument doc(m_data);
    QString json_string = doc.toJson(QJsonDocument::Compact);
    QString escaped_json = json_string;

    escaped_json.replace("\\", "\\\\");
    escaped_json.replace("'", "\\");

    QVBoxLayout* main_layout = new QVBoxLayout(this);

    m_web_view = new QWebEngineView(this);
    m_web_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(m_web_view, &QWebEngineView::loadFinished, [this, escaped_json](bool ok){
        if (ok) {
            QString js_call = QString("render_report('%1');").arg(escaped_json);
            m_web_view->page()->runJavaScript(js_call);
        }
    });

    main_layout->addWidget(m_web_view);

    QString report_file = QString("qrc:/reports/build/html/%1").arg(m_report_file);
    m_web_view->load(QUrl(report_file));


}

