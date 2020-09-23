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

    void loadSchedule(const QDate& date);
    void setMdiArea(QMdiArea* mdi);
    void setupScheduleGrid();
    void populateScheduleGrid(int hr);

private slots:
    void scheduleDateChanged(const QDate& date);
    void createBreaks();
    void hourClicked(QListWidgetItem* item);

private:
    Ui::ScheduleForm *ui;
    std::unique_ptr<EntityDataModel> edmSchedule;
    QMdiArea* mMdiArea;
};

#endif // SCHEDULEFORM_H
