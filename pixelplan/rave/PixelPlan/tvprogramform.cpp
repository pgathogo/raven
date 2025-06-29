#include <ranges>

#include <QListWidgetItem>
#include <QDebug>

#include "tvprogramform.h"
#include "ui_tvprogramform.h"


#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "../../../rave/framework/ui_baseentitybrowserdlg.h"

#include "tvprogram.h"

TVProgramForm::TVProgramForm(std::shared_ptr<PIXELPLAN::TVProgram> tvprogram, QDialog* parent) :
    BaseEntityDetailDlg(parent)
    ,ui(new Ui::TVProgramForm)
    ,m_tvprogram(tvprogram)
{

    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    populate_weekdays();
    populateFormWidgets();

    ui->twMain->setCurrentIndex(0);
}

void TVProgramForm::populate_weekdays()
{
    std::vector<QString> weekdays = {"Mon", "Tue", "Wed", "Thur", "Fri", "Sat", "Sun"};

    for ( auto dow: weekdays) {
        QListWidgetItem* twi = new QListWidgetItem(dow);
        twi->setCheckState(Qt::CheckState::Unchecked);
        ui->lwDays->addItem(twi);
    }


}

QString TVProgramForm::weekdays_to_str()
{
    QString dow = "";

    for(int i=0; i<ui->lwDays->count(); ++i) {
        QListWidgetItem* twi = ui->lwDays->item(i);
        if (twi->checkState() == Qt::CheckState::Checked) {

            if (dow == ""){
                dow += twi->text();
            } else {
                dow += ","+twi->text();
            }
        }
    }
    return dow;
}


void TVProgramForm::make_dow_selection(const QString dow_str)
{
    if (dow_str.isEmpty())
        return;

    QStringList str_list =  dow_str.split(",");

    for(int i=0; i< ui->lwDays->count(); ++i){
        QListWidgetItem* twi = ui->lwDays->item(i);
        if (str_list.contains(twi->text())) {
            twi->setCheckState(Qt::CheckState::Checked);
        }
    }
}



TVProgramForm::~TVProgramForm()
{
    delete ui;
}


ActionResult TVProgramForm::saveRecord()
{
    populateEntityFields();
    auto ar = m_tvprogram->validate();
    return ar;
}

std::string TVProgramForm::windowTitle()
{
    return "TV Program";
}

void TVProgramForm::populateEntityFields()
{
    m_tvprogram->set_title(ui->edtTitle->text().toStdString());
    m_tvprogram->set_description(ui->edtDescription->toPlainText().toStdString());

    m_tvprogram->set_start_time(ui->edtStartTime->time());
    m_tvprogram->set_end_time(ui->edtEndTime->time());

    m_tvprogram->set_duration(ui->sbDuration->value());
    m_tvprogram->set_producer(ui->edtProducer->text().toStdString());
    m_tvprogram->set_director(ui->edtDirector->text().toStdString());
    m_tvprogram->set_writer(ui->edtWriter->text().toStdString());
    m_tvprogram->set_presenter(ui->edtPresenter->text().toStdString());
    m_tvprogram->set_cast(ui->edtCast->toPlainText().toStdString());

    m_tvprogram->set_broadcast_days(weekdays_to_str().toStdString());


}

void TVProgramForm::populateFormWidgets()
{

    ui->edtTitle->setText(m_tvprogram->title()->to_qstring());
    ui->edtDescription->setText(m_tvprogram->description()->to_qstring());

    ui->edtStartTime->setTime(QTime(m_tvprogram->start_time()->value().fromString("HH:mm")));
    ui->edtEndTime->setTime(QTime(m_tvprogram->end_time()->value().fromString("HH:mm")));

    ui->sbDuration->setValue(m_tvprogram->duration()->value());

    ui->edtProducer->setText(m_tvprogram->producer()->to_qstring());
    ui->edtDirector->setText(m_tvprogram->director()->to_qstring());
    ui->edtWriter->setText(m_tvprogram->writer()->to_qstring());
    ui->edtPresenter->setText(m_tvprogram->presenter()->to_qstring());
    ui->edtCast->setText(m_tvprogram->cast()->to_qstring());

    make_dow_selection(m_tvprogram->broadcast_days()->to_qstring());

}

void TVProgramForm::clear_widgets()
{}
