#include <memory>
#include <QDebug>
#include "../framework/ravenexception.h"
#include "../utils/tools.h"

#include "../Traffik/orderbooking.h"

#include "bookedaudiocheckerform.h"
#include "ui_bookedaudiocheckerform.h"

BookedAudioCheckerForm::BookedAudioCheckerForm(std::map<int, std::tuple<std::string, bool>>& selected_audios, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::BookedAudioCheckerForm)
{
    ui->setupUi(this);
    setWindowTitle("Booked Audio Analysis");
    connect(ui->btnSave, &QPushButton::clicked, this, &BookedAudioCheckerForm::save);
    connect(ui->btnCancel, &QPushButton::clicked, this, &BookedAudioCheckerForm::cancel);
    find_booked_audio(selected_audios);
}

BookedAudioCheckerForm::~BookedAudioCheckerForm()
{
    delete ui;
}

void BookedAudioCheckerForm::find_booked_audio(std::map<int, std::tuple<std::string, bool>>& audios)
{
    std::string ids;
    for (auto [id, data]: audios) {
        if(!ids.empty())
            ids +=",";
        ids += std::to_string(id);
    }

    QString DATE_FORMAT = "yyyy-MM-dd";
    QString filter_date = QDate::currentDate().toString(DATE_FORMAT);

    std::stringstream sql;
    sql << " select rave_audio.id, rave_audio.title, rave_client.name, rave_audio.audio_type, "
        << " rave_orderbooking.book_date, rave_orderbooking.book_time "
        << " From rave_audio, rave_orderbooking, rave_spot, rave_spotaudio, rave_client "
        << " Where rave_audio.id = rave_spotaudio.audio_id "
        << " and rave_spot.id = rave_spotaudio.spot_id "
        << " and rave_orderbooking.spot_id = rave_spot.id "
        << " and rave_client.id = rave_spot.client_id "
        << " and rave_orderbooking.book_date >= '"+filter_date.toStdString()+"'"
        << " and rave_spotaudio.audio_id in ("+ids+")";

    try{
        int count = m_edm.readRaw(sql.str());
        //if (count > 0){
        gather_data();
        show_booking_summary();
        show_zero_booking(audios);
       // }
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

}

void BookedAudioCheckerForm::gather_data()
{
    auto provider = m_edm.getDBManager()->provider();
    if (provider->cacheSize() == 0)
        return;

    provider->cache()->first();
    std::string field_name;
    std::string field_value;

    do{
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        DetailData dd;

        for(; it_begin != it_end; ++it_begin){
            field_name = (*it_begin).first;
            field_value = (*it_begin).second;

            if (field_name == "id" )
                dd.audio_id = std::stoi(field_value);

            if (field_name == "title")
                dd.audio_title = field_value;

            if (field_name == "audio_type")
                dd.audio_type = field_value;

            if (field_name == "book_date")
                dd.book_date = field_value;

            if (field_name == "book_time")
                dd.book_time = field_value;
        }

        //auto& [title, counter] = m_summary_data[dd.audio_id];
        if (m_summary_data.find(dd.audio_id) == m_summary_data.end()){
            m_summary_data[dd.audio_id] = std::make_tuple(dd.audio_title, 1);
            qDebug() << "Not Found: "<< dd.audio_id;
        }else{
            auto& [title, counter] = m_summary_data[dd.audio_id];
            counter += 1;
            qDebug() << "Found "<< dd.audio_id;
        }

        m_detail_data.push_back(dd);

        provider->cache()->next();
    } while (!provider->cache()->isLast());

}

void BookedAudioCheckerForm::show_booking_summary()
{
  ui->twData->setColumnCount(2);

  for (auto& [id, data] : m_summary_data){
      auto& [name, counter] = data;
      QTableWidgetItem* name_item = new QTableWidgetItem(QString::fromStdString(name));
      QTableWidgetItem* counter_item = new QTableWidgetItem(QString::number(counter));
      name_item->setData(Qt::UserRole, id);
      ui->twData->insertRow(ui->twData->rowCount());
      ui->twData->setItem(ui->twData->rowCount()-1, 0, name_item);
      ui->twData->setItem(ui->twData->rowCount()-1, 1,counter_item);
  }

}

void BookedAudioCheckerForm::show_zero_booking(std::map<int, std::tuple<std::string, bool>>& audios)
{
  for(auto& [key, data] : audios){
    auto& [title, ok_to_delete] = data;
    if (m_summary_data.find(key) == m_summary_data.end()){
        QTableWidgetItem* name_item = new QTableWidgetItem(QString::fromStdString(title));
        QTableWidgetItem* counter_item = new QTableWidgetItem("0");

        name_item->setData(Qt::UserRole, -1);
        ui->twData->insertRow(ui->twData->rowCount());
        ui->twData->setItem(ui->twData->rowCount()-1, 0, name_item);
        ui->twData->setItem(ui->twData->rowCount()-1, 1, counter_item);
    }else{
        ok_to_delete = false;
    }

  }

}

void BookedAudioCheckerForm::save()
{
    done(1);
}

void BookedAudioCheckerForm::cancel()
{
    done(0);
}
