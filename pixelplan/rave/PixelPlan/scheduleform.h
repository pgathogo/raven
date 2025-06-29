#ifndef SCHEDULEFORM_H
#define SCHEDULEFORM_H

#include <memory>
#include <QDialog>

namespace Ui {
class ScheduleForm;
}

class EntityDataModel;
class QMdiArea;
class QListWidgetItem;

class ScheduleForm : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleForm(QWidget *parent = nullptr);
    ~ScheduleForm();

    void load_schedule(const QDate& date);
    void setMdiArea(QMdiArea* mdi);
    void delete_all_empty_breaks(QDate, int);
    void clear_schedule();

private slots:
    void create_breaks();
    void delete_breaks();
    void hour_clicked(QListWidgetItem* item);
    void schedule_date_changed(const QDate& date);

private:
    Ui::ScheduleForm *ui;
    std::unique_ptr<EntityDataModel> m_edm_schedule;
    QMdiArea* mMdiArea;

    void build_tree_view();
    QDate current_date();
};

#endif // SCHEDULEFORM_H
