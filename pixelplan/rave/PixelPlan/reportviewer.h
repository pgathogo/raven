#pragma once

#include <QDialog>
#include <QJsonObject>

class QMdiArea;
class QWebEngineView;


class ReportViewer : public QDialog
{
   Q_OBJECT

public:
   ReportViewer(QJsonObject, QString, QMdiArea*);

private:
    void setupui();
    void render_report();

    QJsonObject m_data;
    QString m_report_file;
    QWebEngineView* m_web_view;


};

