#include <QMdiArea>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QDate>
#include <QTreeView>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>

#include "viewbreaksform.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravenexception.h"


namespace PIXELPLAN
{
    ViewBreaksForm::ViewBreaksForm(QWidget* parent)
    :QDialog(parent)
    ,m_edm_breaks{nullptr}
    ,m_mdi_area{nullptr}
    {
        setup_ui();
        m_breaks_model = new ViewBreaksModel;
        m_breaks->setModel(m_breaks_model);
        setWindowTitle("View Breaks");
    }

    void ViewBreaksForm::setup_ui()
    {
        setMinimumSize(850, 500);

        auto* lblFrom = new QLabel("From Date:");
        m_dtFrom = new QDateEdit();
        m_dtFrom->setCalendarPopup(true);

        auto* hlFrom = new QHBoxLayout;
        hlFrom->addWidget(lblFrom);
        hlFrom->addWidget(m_dtFrom);

        auto* lblTo = new QLabel("To Date:");
        m_dtTo = new QDateEdit();
        m_dtTo->setCalendarPopup(true);

        auto* hlTo = new QHBoxLayout;
        hlTo->addWidget(lblTo);
        hlTo->addWidget(m_dtTo);

        m_dtFrom->setDate(QDate::currentDate());
        m_dtTo->setDate(QDate::currentDate());

        auto* btnViewBreaks = new QPushButton("View Breaks");
        connect(btnViewBreaks, &QPushButton::clicked, this, &ViewBreaksForm::on_view_breaks);

        QHBoxLayout* hl_topbar = new QHBoxLayout;
        hl_topbar->setSpacing(15);
        hl_topbar->addLayout(hlFrom);
        hl_topbar->addLayout(hlTo);
        hl_topbar->addWidget(btnViewBreaks);
        hl_topbar->addStretch(1);

        auto* cbExpand = new QCheckBox("Expand All");
        connect(cbExpand, &QCheckBox::checkStateChanged, this, &ViewBreaksForm::on_expand_all);
        auto* hl_expand = new QHBoxLayout;
        hl_expand->addWidget(cbExpand);

        m_breaks = new QTreeView;
        prepare_tree(m_breaks);

        auto* main_layout = new QVBoxLayout;
        main_layout->addLayout(hl_topbar);
        main_layout->addLayout(hl_expand);
        main_layout->addWidget(m_breaks);

        setLayout(main_layout);

    }

    void ViewBreaksForm::on_view_breaks(bool clicked)
    {
        if (m_dtTo->date() < m_dtFrom->date()) {
            QMessageBox::critical(this, tr("View Breaks"),
                        tr("`To` date is LESS than `From` date"),
                        QMessageBox::Ok );
            return;
        }

        view_breaks(m_dtFrom->date(), m_dtTo->date());
    }

    void ViewBreaksForm::on_expand_all(Qt::CheckState state)
    {
        (state == Qt::Checked ) ? m_breaks->expandAll() : m_breaks->collapseAll();
    }

    void ViewBreaksForm::prepare_tree(QTreeView* tree_view)
    {
       tree_view->setColumnWidth(0, 200);
       tree_view->setColumnWidth(1, 100);
       tree_view->setColumnWidth(2, 100);
       tree_view->setColumnWidth(3, 100);
       tree_view->setColumnWidth(4, 100);
       tree_view->setColumnWidth(5, 100);
       tree_view->setColumnWidth(6, 150);

       tree_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    }

    void ViewBreaksForm::view_breaks(const QDate& from_date, const QDate& to_date)
    {
        m_breaks_model->clear_data();

        std::string str_from_date = from_date.toString("yyyy-MM-dd").toStdString();
        std::string str_to_date = to_date.toString("yyyy-MM-dd").toStdString();

    std::string sql = std::format(
        "SELECT "
        "rave_schedule.id, rave_schedule.schedule_date, rave_schedule.schedule_time, rave_schedule.schedule_hour,"
        "rave_schedule.fade_in, rave_schedule.fade_out, rave_schedule.fade_delay, rave_schedule.play_status, rave_schedule.play_date,"
        "rave_schedule.play_time, rave_schedule.auto_transition, rave_schedule.live_transition, rave_schedule.schedule_item_type,"
        "rave_schedule.break_duration, rave_schedule.break_start_win, rave_schedule.break_end_win, rave_schedule.break_max_spots,"
        "rave_schedule.break_fill_method, rave_schedule.booked_spots, rave_schedule.break_duration_left, rave_schedule.break_mode,"
        "rave_schedule.break_status, rave_schedule.comment, rave_schedule.audio_id, rave_schedule.break_layout_line_id "
        " FROM rave_schedule, rave_breaklayoutline, rave_breaklayout"
        " WHERE rave_schedule.break_layout_line_id = rave_breaklayoutline.id"
        " AND rave_breaklayoutline.break_layout_id = rave_breaklayout.id"
        " AND rave_breaklayout.deleted = 0"
        " AND rave_schedule.schedule_date between '{}' AND '{}'"
        " AND rave_schedule.schedule_item_type = 'COMM-BREAK'"
        " ORDER BY rave_schedule.schedule_date, rave_schedule.schedule_hour,"
        " rave_schedule.schedule_time ", str_from_date, str_to_date );

      EntityDataModel edm;

    try {

      int record_count = edm.readRaw(sql);

      if (record_count == 0)
          return;

    } catch (PostgresException& pe) {
      qDebug() << QString::fromStdString(pe.errorMessage()) << '\n';
        std::cerr << pe.errorMessage() << '\n';

    }

    auto method_type = [](std::string f_m) -> std::string {
       if (f_m == "")
         return f_m;

    std::map<std::string, std::string> method_map;
    method_map["R"] = "Random";
    method_map["S"] = "Sequence";

    return method_map[f_m];
  };

  //Breaks comm_breaks;
  OrderedMap comm_breaks;

  auto provider = edm.getDBManager()->provider();
  if (provider->cacheSize() == 0)
      return;

  provider->cache()->first();

  do {

      auto itB = provider->cache()->currentElement()->begin();
      auto itE = provider->cache()->currentElement()->end();

      PPBreak comm_break;

      for(; itB != itE; ++itB)
      {
          std::string field_name = (*itB).first;
          std::string field_value = (*itB).second;

          if (field_name == "id")
          comm_break.id = std::stoi(field_value);

          if (field_name == "schedule_date")
              comm_break.schedule_date = field_value;

          if (field_name == "schedule_hour")
              comm_break.schedule_hour = std::stoi(field_value);

          if (field_name == "comment")
              comm_break.comment = field_value;

          if (field_name == "schedule_time")
              comm_break.schedule_time = field_value;

          if (field_name == "break_mode")
              comm_break.break_mode = field_value;

          if (field_name == "break_fill_method")
              comm_break.break_fill_method = method_type(field_value);

          if (field_name == "break_max_spots")
              comm_break.max_spots = std::stoi(field_value);

          if (field_name == "break_duration")
              comm_break.break_duration = std::stoi(field_value);

          if (field_name == "booked_spots")
              comm_break.booked_spots = std::stoi(field_value);

          if (field_name == "break_duration_left")
              comm_break.time_left = std::stoi(field_value);

      }

        comm_breaks.insert(comm_break.schedule_date, comm_break.schedule_hour, comm_break);

    provider->cache()->next();

  } while(!provider->cache()->isLast());

  // Show Items in a TreeView
  build_tree_view(comm_breaks);

 }

void ViewBreaksForm::build_tree_view(OrderedMap& comm_breaks)
{
    // m_sched_model->clear();

  if (comm_breaks.insertion_order().size() > 0)
  {
    // m_breaks->setModel(m_sched_model);
   // ui->tvSchedule->header()->setSectionResizeMode(QHeaderView::Stretch);

    m_breaks->setColumnWidth(0, 200);
    m_breaks->setColumnWidth(1, 100);
    m_breaks->setColumnWidth(2, 100);
    m_breaks->setColumnWidth(3, 100);
    m_breaks->setColumnWidth(4, 100);
    m_breaks->setColumnWidth(5, 100);
    m_breaks->setColumnWidth(6, 150);

    m_breaks_model->make_tree(comm_breaks);

  }

}

void ViewBreaksForm::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;

}

}