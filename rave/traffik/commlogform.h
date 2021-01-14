#ifndef COMMLOGFORM_H
#define COMMLOGFORM_H

#include <QDialog>
#include "commlogtreeviewmodel.h"

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

private slots:
    void comm_log_date_changed();
    void comm_log_hours_changed(int i);
    void change_view_mode(int state);
    void select_all_hours(int state);
    void hours_popup_hidden();

private:
    Ui::CommLogForm *ui;
    QMdiArea* m_mdi_area;
    QCheckList* m_hours;
    std::vector<QStandardItem*> m_check_items;
    CommercialLogs m_comm_logs;

    void set_hour_combobox();
    void fetch_bookings();
};

#endif // COMMLOGFORM_H
