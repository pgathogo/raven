#ifndef TVPROGRAMFORM_H
#define TVPROGRAMFORM_H

#include <QDialog>

#include "../../../rave/framework/baseentitydetaildlg.h"

namespace Ui {
class TVProgramForm;
}

namespace PIXELPLAN {
class TVProgram;
}

class TVProgramForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit TVProgramForm(std::shared_ptr<PIXELPLAN::TVProgram>, QDialog* parent = nullptr);
    ~TVProgramForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;
    void clear_widgets() override;
    void show_message();

private slots:
    void on_duration_changed(int);
    void on_end_time_changed(QTime);

private:

    Ui::TVProgramForm *ui;

    std::shared_ptr<PIXELPLAN::TVProgram> m_tvprogram;


    void populate_weekdays();
    QString weekdays_to_str();
    void make_dow_selection(const QString);


};

#endif // TVPROGRAMFORM_H
