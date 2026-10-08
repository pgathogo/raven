#pragma once

#include <QDialog>
#include <QJsonObject>

class QMdiArea;
class QWebEngineView;
class QVBoxLayout;


class ReportViewer : public QDialog
{
   Q_OBJECT

public:
   ReportViewer(QJsonObject, QString, QMdiArea*, bool show_dev_tools=false);

private slots:
   void on_save_pdf();
    void on_pdf_print_finished(const QString&, bool);

private:
    void setupui();
    void render_report();

    QJsonObject m_data;
    QString m_report_file;
    QWebEngineView* m_web_view;
    QVBoxLayout* m_main_layout;

    bool m_show_dev_tools;


};

