#ifndef COMMLOGANALYZER_H
#define COMMLOGANALYZER_H

#include <QDialog>
#include <QFile>
#include <QDate>

#include <tuple>

namespace Ui {
class CommLogAnalyzer;
}

class QMdiArea;
class RavenSetup;

class CommLogAnalyzer : public QDialog
{
    Q_OBJECT

public:
    explicit CommLogAnalyzer(QWidget *parent = nullptr);
    ~CommLogAnalyzer();

    void setMdiArea(QMdiArea*);

private slots:
    void on_load_log_file(const QDate&);

private:
    void parse_and_display(const QString&);
    int count_lines(QFile&);
    std::tuple<std::string, std::string, std::string> parse_timestamp(const std::string&);
    QString find_log_file_by_date(const QString&, const QDate&);

    Ui::CommLogAnalyzer *ui;
    QMdiArea* m_mdi_area;

    std::unique_ptr<EntityDataModel> m_edm_setup;
    std::shared_ptr<RavenSetup> m_setup;
};

#endif // COMMLOGRECONCILER_H
