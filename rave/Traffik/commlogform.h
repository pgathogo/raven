#ifndef COMMLOGFORM_H
#define COMMLOGFORM_H

#include <QDialog>
#include "commlogtreeviewmodel.h"
#include "../framework/datetimeselector.h"

namespace Ui {
class CommLogForm;
}

class QMdiArea;
class QCheckList;

class CommLogForm : public QDialog
{
    Q_OBJECT

public:
    explicit CommLogForm(QWidget *parent = nullptr);
    ~CommLogForm();
    void setMdiArea(QMdiArea* mdi);

    void set_default_dts();

private slots:
    void on_clicked_datetime();
    void comm_log_hours_changed(int i);
    void change_view_mode(int state);
    void select_all_hours(int state);
    void hours_popup_hidden();
    void print_log();

private:
    void set_hour_combobox();
    void fetch_bookings(const DateTimeSelection&);
    std::string comma_sep(const std::vector<int>&);
    void update_hours_label(std::string);
    void write_json_data(QJsonArray&);

    Ui::CommLogForm *ui;
    QMdiArea* m_mdi_area;
    QCheckList* m_hours;
    std::vector<QStandardItem*> m_check_items;
    CommercialLogs m_comm_logs;
    DateTimeSelection m_dts;
};

#endif // COMMLOGFORM_H
