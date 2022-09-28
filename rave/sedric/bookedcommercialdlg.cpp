#include <iostream>

#include "bookedcommercialdlg.h"
#include "ui_bookedcommercialdlg.h"

#include "../framework/entitydatamodel.h"
#include "../data/commercialviewer.h"
#include "../utils/tools.h"

BookedCommercialDlg::BookedCommercialDlg(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::BookedCommercialDlg)
{
    ui->setupUi(this);

    m_comm_viewer = std::make_unique<DATA::CommercialViewer>(this);
    ui->vlView->addWidget(m_comm_viewer.get());

    connect(ui->btnClose, &QPushButton::clicked, this, &BookedCommercialDlg::close_dialog);

    setWindowTitle("Commercial Break(s) Viewer");
}

BookedCommercialDlg::~BookedCommercialDlg()
{
    delete ui;
}

void BookedCommercialDlg::close_dialog()
{
    done(1);
}

void BookedCommercialDlg::show_booked_commercial(int break_id)
{
    m_comm_viewer->clear();
    m_comm_viewer->create_view_headers();

    std::stringstream sql;

    sql <<"select a.id, a.schedule_date, a.schedule_time, a.schedule_hour, "
        <<" b.schedule_id, b.spot_id,   "
        <<" c.name spot_title, c.spot_duration, c.real_duration, c.client_id,   "
        <<" d.audio_id, "
        <<" e.name client_name   "
        <<" from rave_schedule a "
        <<" left outer join rave_orderbooking b on a.id = b.schedule_id "
        <<" left outer join rave_spot c on b.spot_id = c.id "
        <<" left outer join rave_spotaudio d on c.id = d.spot_id "
        <<" left outer join rave_client e on c.client_id = e.id ";

      std::string where_filter = " where a.id = "+ std::to_string(break_id);

      sql << where_filter;

      EntityDataModel edm;
      edm.readRaw(sql.str());

      auto provider = edm.getDBManager()->provider();

      if (provider->cacheSize() == 0)
          return;

      QList<QStandardItem*> columns;

      std::string spot_title;
      std::string client_name;
      std::string spot_duration;
      std::string schedule_time;

      provider->cache()->first();

      do{
          auto it_begin = provider->cache()->currentElement()->begin();
          auto it_end = provider->cache()->currentElement()->end();

          for(; it_begin != it_end; ++it_begin) {

            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "spot_title"){
                spot_title = field_value;
            }

            if (field_name == "spot_duration"){
                spot_duration = field_value;
            }

            if (field_name == "client_name"){
                client_name = field_value;
            }

            if (field_name == "schedule_time" && schedule_time.empty()){
                schedule_time = field_value;
            }
        }

          auto title = new QStandardItem(QString::fromStdString(spot_title));
          auto client = new QStandardItem(QString::fromStdString(client_name));
          auto duration = new QStandardItem(QString::fromStdString(spot_duration));

          columns.append(title);
          columns.append(client);
          columns.append(duration);

          m_comm_viewer->model()->appendRow(columns);
          columns.clear();

          provider->cache()->next();
      } while (!provider->cache()->isLast());

      QTime break_time = QTime::fromString(stoq(schedule_time));
      m_comm_viewer->set_title("Commercial Break: "+break_time.toString("HH:mm"));

      QFont font("Arial", 12, QFont::Bold);
      m_comm_viewer->set_title_font(font);
}
