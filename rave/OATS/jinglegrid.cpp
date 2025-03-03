#include <chrono>
#include <limits>
#include <ranges>

#include <QGridLayout>
#include <QButtonGroup>
#include <QAction>
#include <QFileDialog>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>
#include <QProgressBar>
#include <QTimer>
#include <QPainter>

#include <QVBoxLayout>
#include <QPalette>

#include "jinglegrid.h"

#include "../audio/trackbrowser.h"
#include "../audio/trackpickerdialog.h"
#include "../audio/audio.h"


using namespace std::chrono_literals;

namespace OATS
{
    Jingle::Jingle()
        :m_page{-1}
        ,m_row{-1}
        ,m_col{-1}
        ,m_title{""}
        ,m_jingle_id{""}
        ,m_jingle_status{Jingle::JingleStatus::Ready}
    ,m_audio{ std::make_shared<AUDIO::Audio>() }
    {
    }

    Jingle::Jingle(int page, int row, int col, QString title)
        :m_page{page}
        ,m_row{row}
        ,m_col{col}
        ,m_title{title}
        ,m_jingle_status{Jingle::JingleStatus::Ready}
        ,m_audio{std::make_shared<AUDIO::Audio>()}
    {
        m_jingle_id = QString::number(page)+
               QString::number(row)+
               QString::number(col);
    }

    Jingle::~Jingle()
    {
        //if (m_audio != nullptr)
         //   delete m_audio;
    }

    QString Jingle::title() const{
        //return m_title;
        return m_audio->title()->to_qstring();
    }

    bool Jingle::is_empty()
    {
        return m_audio->title()->to_qstring().isEmpty();
    }

    void Jingle::set_title(const QString title)
    {
        m_title = title;
    }

    int Jingle::page() const
    {
        return m_page;
    }

    void Jingle::set_page(int id)
    {
        m_page = id;
    }

    int Jingle::row() const
    {
        return m_row;
    }

    int Jingle::col() const
    {
        return m_col;
    }

    void Jingle::set_row(int r)
    {
        m_row = r;
    }

    void Jingle::set_col(int c)
    {
        m_col = c;
    }

    QString Jingle::jingle_id() const
    {
        return m_jingle_id;
    }

    void Jingle::set_jingle_id(const QString id)
    {
        m_jingle_id = id;
    }

    /*
    int Jingle::track_id()
    {
        return m_track_id;
    }

    void Jingle::set_track_id(int t_id)
    {
        m_track_id = t_id;
    }

    QString Jingle::track_path()
    {
        return m_track_path;
    }

    void Jingle::set_track_path(QString t_path)
    {
        m_track_path = t_path;
    }

    int Jingle::track_duration()
    {
        return m_track_duration;
    }

    void Jingle::set_track_duration(int duration)
    {
        m_track_duration = duration;
    }
    */

    void Jingle::set_jingle_status(Jingle::JingleStatus status)
    {
        m_jingle_status = status;
    }

    void Jingle::set_audio(std::shared_ptr<AUDIO::Audio> audio)
    {
       m_audio = audio;
    }

    std::shared_ptr<AUDIO::Audio> Jingle::audio()
    {
        return m_audio;
    }

    /* ---------------- GridButton ----------------------------- */

   // GridButton::GridButton(int id)
    GridButton::GridButton(int page, int row, int col)
        :m_page{page}
        ,m_row{row}
        ,m_col{col}
        ,m_jingle{new Jingle()}
        ,m_count_down_timer{nullptr}
    {
        m_count_down_timer = std::make_unique<QTimer>(this);
        connect(m_count_down_timer.get(), &QTimer::timeout, this, &GridButton::count_down);
        setCheckable(true);
        auto name = QString("btn_%1_%2_%3").arg(m_page).arg(m_row).arg(m_col);
        this->setObjectName(name);
    }

    GridButton::GridButton(int page, Jingle* const jingle)
        :m_page{page}
         ,m_jingle{jingle}
    {
        setText(QString::fromStdString(jingle->audio()->title()->value()));
    }

    GridButton::~GridButton()
    {
    }

    Jingle* GridButton::jingle()
    {
        return m_jingle;
    }

    void GridButton::set_jingle(Jingle* const jingle)
    {
        (jingle == nullptr) ? setText("") :  setText(jingle->audio()->title()->to_qstring()) ;
        m_jingle = jingle;
    }

    void GridButton::start_countdown_timer()
    {
      auto COUNT_DOWN_INTERVAL = 50ms;
      m_start_tick_count = m_audio_tool.get_tick_count();
      m_count_down_timer->start(COUNT_DOWN_INTERVAL);
    }

    void GridButton::count_down()
    {
        auto trigger_tick = m_audio_tool.get_tick_count();
        int elapsed = trigger_tick - m_start_tick_count;
        int remaining = m_jingle->audio()->duration()->value() - elapsed;

        auto progress_value = (100 - round((float)remaining/
                                           m_jingle->audio()->duration()->value() * 100));
        m_color_value = progress_value / 100;

        /*
        setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
                                               "stop:0 rgba(230, 103, 192, 255), stop:%1 rgba(230, 103, 192, 255),"
                                               "stop:%2 rgba(79, 179, 228, 255), stop:1 rgba(79, 179, 228, 255));")
                                                .arg(m_color_value-0.01f)
                                                .arg(m_color_value));

        if ( (progress_value >= 100) || (m_color_value >= 1.0f)){
            m_count_down_timer->stop();
        }
        */

       if (progress_value >= 100){
           m_count_down_timer->stop();
           switch_off();
       }

    }

    void GridButton::stop_countdown_timer()
    {
        m_count_down_timer->stop();
    }

    void GridButton::switch_on()
    {
        set_pixmap(QPixmap(":/images/icons/live_dot.png"));
        update(rect());
    }

    void GridButton::switch_off()
    {
        remove_indicator();
        update(rect());
        setChecked(false);
    }

    void GridButton::remove_indicator()
    {
        set_pixmap(QPixmap());
    }

    void GridButton::set_pixmap(const QPixmap& pixmap)
    {
        m_pixmap = pixmap;
    }

    QSize GridButton::sizeHint() const
    {
       #undef max
       const auto parent_hint = QPushButton::sizeHint();
       // add margins here if neeeded
       return QSize(parent_hint.width() + m_pixmap.width(), std::max(parent_hint.height(), m_pixmap.height()));
    }

    void GridButton::paintEvent(QPaintEvent* e)
    {
        QPushButton::paintEvent(e);
        if (!m_pixmap.isNull())
        {
            const int y = (height() - m_pixmap.height())/2;
            QPainter painter(this);
            //painter.drawPixmap(5, y, m_pixmap);
            painter.drawPixmap(width()-20, y, m_pixmap);
        }
    }

    /* ----------------------------------------------- */

    JingleGrid::JingleGrid(QWidget *parent)
        : QWidget{parent}
        ,m_main_layout{nullptr}
        ,m_pager_layout{nullptr}
        ,m_context_menu{nullptr}
        ,m_load_action{nullptr}
        ,m_jingle_filename{""}
        ,m_current_grid_button{ new GridButton(0,0,0) }
    {
        m_main_layout = std::make_unique<QVBoxLayout>();
        m_grid_layout = new QGridLayout();

        make_toolbar(m_main_layout.get());

        m_file_path = std::make_unique<QLabel>("Jingle fiile... ");
        m_file_path->setStyleSheet("color:#0479B0;");
        m_main_layout->addWidget(m_file_path.get());


        make_grid_cells();
        make_jingle_view();

        make_stop_button(m_main_layout.get());
        make_pager(m_main_layout.get());

        m_track_picker_dialog = std::make_unique<AUDIO::TrackPickerDialog>();
        connect(m_track_picker_dialog.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &JingleGrid::selected_audio2);
        connect(m_track_picker_dialog.get(), &AUDIO::TrackPickerDialog::finished, this, &JingleGrid::close_track_picker);

        //attach_jingle_to_buttons(JINGLE_PAGE_ONE);

        m_main_layout->setStretch(2, 1);

        this->setLayout(m_main_layout.get());

    }

    QGridLayout* JingleGrid::layout() const
    {
        return m_grid_layout;
    }

    void JingleGrid::print_grid_cells()
    {
        for(auto page : std::views::iota(0, PAGE_COUNT)) {
           for(auto row: std::views::iota(0, GRID_ROWS)) {
                for (auto col : std::views::iota(0, GRID_COLS)) {

                    auto title = m_grid_cells[page][row][col]->title();

                    QString m = QString("Page: %1  Row: %2  Col: %3 Jingle= %4")
                                    .arg(QString::number(page))
                                    .arg(QString::number(row))
                                    .arg(QString::number(col))
                                    .arg(title);

                    qDebug() << m;

                }
           }

       }

    }

    void JingleGrid::make_grid_cells()
    {
        for(auto page : std::views::iota(0, PAGE_COUNT)) {
           for(auto row: std::views::iota(0, GRID_ROWS)) {
                for (auto col : std::views::iota(0, GRID_COLS)) {

                   auto jingle = std::make_unique<Jingle>(page, row, col, "");

                    m_grid_cells[page][row][col] = std::move(jingle);

                }
           }

       }

    }

    void JingleGrid::make_jingle_view()
    {
        QString btn_style(
            "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );"
            "border-radius: 20px;"
            "border-style: inset;"
            "border-bottom-width: 2px;"
            "border-radius: 3px;"
            "border-bottom-color:#374148;"
            "color:#FFFFFF;"
            "height:35;"
            "font-weight:bold;}"

            "QPushButton:hover{"
            "background-color:#555D64; "
            "border-width:1px;"
            "border-color:#0479B0;"
            " }"

            "QPushButton:pressed{background-color:#555D64; border:none;}"
            );

       m_grid_layout = new QGridLayout(this);

       int id=1;

       for(auto row: std::views::iota(0, GRID_ROWS)) {

          for (auto col : std::views::iota(0, GRID_COLS)) {

              GridButton* grid_button = new GridButton(id++, row, col);

              grid_button->setStyleSheet(btn_style);

              grid_button->setContextMenuPolicy(Qt::CustomContextMenu);

              connect(grid_button, &GridButton::customContextMenuRequested, this,
                      [=](const QPoint& pos){ open_menu_at(row, col, pos); } );

              connect(grid_button, &GridButton::clicked, this,
                      [=](){grid_button_clicked(row, col);});

               m_grid_layout->addWidget(grid_button, row, col, 1, 1);
           }
        }

       m_grid_layout->setVerticalSpacing(2);
       QWidget* button_widget = new QWidget(this);
       button_widget->setLayout(m_grid_layout);
       m_main_layout->addWidget(button_widget);

    }

    void JingleGrid::grid_button_clicked(int row, int col)
    {
        m_current_grid_button = dynamic_cast<GridButton*>(m_grid_layout->itemAtPosition(row, col)->widget());

        if (m_is_track_picker_open) {

            emit m_track_picker_dialog->selected_audio(
                 const_cast<AUDIO::TrackBrowser&>(m_track_picker_dialog->track_browser()).current_selected_audio());
            return;
        }

        if (m_current_grid_button->text().isEmpty())
            return;

        print("AAAA");
        m_current_grid_button->set_page(m_current_page);
        print("BBB");

        emit play_jingle(m_grid_cells[m_current_page][row][col].get());

        m_current_grid_button->switch_on();
        m_play_is_on = true;

        // m_current_grid_button->start_countdown_timer();

    }

   void JingleGrid::stop_play()
    {
       qDebug() << "Output C Stopped!";
       m_current_grid_button->switch_off();
       m_play_is_on = false;
    }

    void JingleGrid::open_menu_at(int row, int col, const QPoint& pos)
    {

       if (m_load_action == nullptr)
       {
        m_load_action = std::make_unique<QAction>("Load...", this);
        m_clear_action = std::make_unique<QAction>("Clear", this);

        m_context_menu = std::make_unique<QMenu>();
        m_context_menu->addAction(m_load_action.get());
        m_context_menu->addAction(m_clear_action.get());

       }

        connect(m_load_action.get(), &QAction::triggered, this,
                [&](){ open_load_dialog(row, col); });

        connect(m_clear_action.get(), &QAction::triggered, this,
                [&](){clear_jingle(row, col);});

        m_context_menu->exec(dynamic_cast<GridButton*>(m_grid_layout->itemAtPosition(row, col)->widget())->mapToGlobal(pos));
    }

    void JingleGrid::open_load_dialog(int row, int col)
    {
        /*
        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(m_grid_buttons[row][col].get());
        m_grid_buttons[row][col]->setGraphicsEffect(effect);
        QPropertyAnimation* p_animation = new QPropertyAnimation(effect, "color");
        p_animation->setStartValue(QColor(Qt::red));
        p_animation->setEndValue(QColor(Qt::green));
        p_animation->setLoopCount(-1);
        p_animation->setDuration(2000);
        p_animation->start();
        */

        // m_current_grid_cell.page = page;
        m_current_grid_cell.row = row;
        m_current_grid_cell.col = col;

//        m_track_picker_dialog = std::make_unique<AUDIO::TrackPickerDialog>();
//        connect(m_track_picker_dialog.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &JingleGrid::selected_audio);

        m_is_track_picker_open = true;
        m_track_picker_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_track_picker_dialog->show();

        m_load_action = nullptr;
        //p_animation->stop();
    }

    void JingleGrid::clear_jingle(int row, int col)
    {
        m_grid_buttons[row][col]->set_jingle(nullptr);
    }

   void JingleGrid::selected_audio2(std::shared_ptr<AUDIO::Audio> audio)
    {
       m_current_grid_button->setText(audio->title()->to_qstring());
       m_current_grid_button->jingle()->set_audio(audio);

    }


    void JingleGrid::close_track_picker(int result)
   {
        m_is_track_picker_open = false;
    }


    void JingleGrid::stop_all()
    {
        emit stop_all_jingles();
        m_play_is_on = false;

    }

    void JingleGrid::clear_buttons()
    {
        for(int row=0; row<= GRID_ROWS-1; ++row){
           for (int col=0; col<GRID_COLS-1; ++col){
                    m_grid_buttons[row][col]->set_jingle(nullptr);
            }
          }
    }

   void JingleGrid::clear_all()
   {
       for(int page=0; page<= PAGE_COUNT; ++page){
            clear_page(page) ;
       }
   }

   void JingleGrid::clear_page(int page)
   {
        for (int row=0; row <= GRID_ROWS-1; ++row){
            for (int col=0; col <= GRID_COLS-1; ++col){
                   m_grid_buttons[row][col]->set_jingle(nullptr);
                   m_grid_buttons[row][col]->remove_indicator();
               }
           }

   }

   void JingleGrid::make_toolbar(QVBoxLayout* main_layout)
   {
        QString btn_style(
        "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );"
        "border-radius: 20px;"
        "border-style: inset;"
        "border-bottom-width: 2px;"
        "border-radius: 3px;"
        "border-bottom-color:#374148;"
        "color:#FFFFFF;"
        "height:45;"
        "width:75;"
        "font-weight:bold;}"

        "QPushButton:hover{"
            "background-color:#555D64; "
            "border-width:1px;"
            "border-color:#0479B0;"
            " }"

        "QPushButton:pressed{"
           "background-color: #555D64;"
           "border:none;}"

        "QPushButton:pressed{background-color:#555D64; border:none;}"
            );

       m_toolbar_layout = new QHBoxLayout();

       m_load_btn = std::make_unique<QPushButton>("Load...");
       m_open_btn = std::make_unique<QPushButton>("Open...");
       m_save_btn = std::make_unique<QPushButton>("Save");
       m_save_as_btn = std::make_unique<QPushButton>("Save As...");
       m_clear_all_btn = std::make_unique<QPushButton>("Clear All");

       m_load_btn->setStyleSheet(btn_style);
       m_open_btn->setStyleSheet(btn_style);
       m_save_btn->setStyleSheet(btn_style);
       m_save_as_btn->setStyleSheet(btn_style);
       m_clear_all_btn->setStyleSheet(btn_style);

       connect(m_load_btn.get(), &QPushButton::clicked, this, &JingleGrid::load_audio);
       connect(m_open_btn.get(), &QPushButton::clicked, this, &JingleGrid::get_jingles);
       connect(m_save_btn.get(), &QPushButton::clicked, this, &JingleGrid::save_jingles);
       connect(m_save_as_btn.get(), &QPushButton::clicked, this, &JingleGrid::save_as);
       connect(m_clear_all_btn.get(), &QPushButton::clicked, this, &JingleGrid::clear_all);

       m_toolbar_layout->addWidget(m_load_btn.get());
       m_toolbar_layout->addSpacerItem(new QSpacerItem(20,40));
       m_toolbar_layout->addWidget(m_open_btn.get());
       m_toolbar_layout->addWidget(m_save_btn.get());
       m_toolbar_layout->addWidget(m_save_as_btn.get());
       m_toolbar_layout->addWidget(m_clear_all_btn.get());
       m_toolbar_layout->addStretch(1);

       m_tool_buttons.push_back(std::move(m_load_btn));
       m_tool_buttons.push_back(std::move(m_open_btn));
       m_tool_buttons.push_back(std::move(m_save_btn));
       m_tool_buttons.push_back(std::move(m_save_as_btn));
       m_tool_buttons.push_back(std::move(m_clear_all_btn));

       m_toolbar_widget = std::make_unique<QWidget>(this);

       m_toolbar_widget->setLayout(m_toolbar_layout);

       main_layout->addWidget(m_toolbar_widget.get());
   }

    void JingleGrid::make_pager(QVBoxLayout* main_layout)
    {
        QString btn_style(
        "QPushButton {background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #555D64 , stop:1 #374148 );"
        "border-style: inset;"
        "border-bottom-width: 2px;"
        "border-bottom-color:#374148;"
        "color:#FFFFFF;"
        "height:20;"
        "font-weight:bold;"
            "}"

        "QPushButton:hover{"
            "background-color:#555D64; "
            "border-width:1px;"
            "border-color:#0479B0;"
            " }"

       "QPushButton:checked {"
        "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, stop:0 #454500 , stop:1 #777700 );"
        "border-style: inset;"
        "border-bottom-width: 1px;"
        "color:#FFFFFF;"
       "font-weight:bold;}"
            );

        m_pager_layout = std::make_unique<QHBoxLayout>();
        m_page_button_group = std::make_unique<QButtonGroup>();


        for(auto page : std::views::iota(0, PAGE_COUNT))
        {
            auto page_button = std::make_unique<QPushButton>(QString::number(page+1));
            page_button->setCheckable(true);
            page_button->setStyleSheet(btn_style);

            connect(page_button.get(), &QPushButton::clicked, this,
                    [=](){switch_page(page);});
                    //[=](){attach_jingle_to_buttons2(page); set_current_page(page);});

            m_page_button_group->addButton(page_button.get());
            m_pager_layout->addWidget(page_button.get());
            m_page_buttons[page] = std::move(page_button);
        }

        m_page_buttons[0]->setChecked(true);
        switch_page(0);

        main_layout->addLayout(m_pager_layout.get());
    }


    int JingleGrid::current_page()
    {
        return m_current_page;
    }

    void JingleGrid::switch_page(int page)
    {
        const auto& rows = m_grid_cells[page];
        for(const auto& [row, cols]: rows) {
            for(const auto& [col, jingle]: cols) {
                GridButton* gb = dynamic_cast<GridButton*>(m_grid_layout->itemAtPosition(row, col)->widget()) ;
                gb->set_jingle(jingle.get());
            }
        }

        qDebug() << "Current Page: "<< page;
        qDebug() << "Grid Button Page: "<< m_current_grid_button->page();

        m_current_page = page;

        if (m_play_is_on && m_current_page != m_current_grid_button->page()) {
            m_current_grid_button->switch_off();
        }

        if (m_play_is_on && m_current_page == m_current_grid_button->page()) {
            m_current_grid_button->switch_on();
        }


    }

   void JingleGrid::make_stop_button(QVBoxLayout* main_layout)
   {
       m_stop_button = std::make_unique<QPushButton>("STOP ALL");
       m_stop_button->setStyleSheet(
           "background-color:#DC143C;"
           "color: #FFFFFF;"
           "height:30;"
           "font-weight:bold;"
           );
       connect(m_stop_button.get(), &QPushButton::clicked, this, &JingleGrid::stop_all);

       main_layout->addWidget(m_stop_button.get());

   }

  void JingleGrid::load_audio()
  {
      open_load_dialog(0,0);
  }

  void JingleGrid::get_jingles()
  {
      m_jingle_filename = QFileDialog::getOpenFileName(this,
                                                       tr("Open jingle file"),
                                                       "",
                                                       tr("Jingles (*.jgl)"));
      if (m_jingle_filename.isEmpty())
          return;

      m_file_path->setText(m_jingle_filename);

      open_from_file(m_jingle_filename);

      switch_page(m_current_page);

      //attach_jingle_to_buttons(m_current_page);
  }

  void JingleGrid::open_from_file(const QString filename)
  {
      QFile jingle_file(filename);
      if (!jingle_file.open(QIODevice::ReadOnly)){
          qWarning("Coudn't open jingle file");
          return;
      }

      QByteArray data = jingle_file.readAll();
      QJsonDocument doc_data(QJsonDocument::fromJson(data));
      QJsonObject jingle_json = doc_data.object();
      QJsonArray jingles = jingle_json["jingles"].toArray();

      for(auto&& jingle_json : jingles) {

         QJsonObject obj = jingle_json.toObject();

         int page = obj["page"].toInt();
         int row = obj["row"].toInt();
         int col = obj["col"].toInt();

         const auto& jingle = m_grid_cells[page][row][col];

         jingle->set_jingle_id(obj["jingle_id"].toString());

         jingle->set_page(obj["page"].toInt());
         jingle->set_row(obj["row"].toInt());
         jingle->set_col(obj["col"].toInt());

         auto audio = std::make_shared<AUDIO::Audio>();

         audio->setId(obj["track_id"].toInt());
         audio->set_title(obj["title"].toString().toStdString());

         QFileInfo fi(obj["track_path"].toString());
         audio->set_file_path(fi.path().toStdString()+"/");


         audio->set_audio_lib_path(obj["track_path"].toString().toStdString());

         audio->set_duration(obj["duration"].toInt());
         audio->set_file_extension(obj["extension"].toString().toStdString());

         jingle->set_audio(std::move(audio));

      }

  }

  QJsonObject JingleGrid::jingle_to_json(std::unique_ptr<Jingle> const& jingle)
  {
     QJsonObject json;

     json["jingle_id"] = jingle->jingle_id();
     json["page"] = jingle->page();
     json["row"] = jingle->row();
     json["col"] = jingle->col();

     json["track_id"] = jingle->audio()->id();
     json["title"] = QString::fromStdString(jingle->audio()->title()->value());
     json["track_path"] = jingle->audio()->full_audio_filename();
     json["extension"] = QString::fromStdString(jingle->audio()->file_extension()->value());
     json["duration"] = jingle->audio()->duration()->value();

     return json;
  }

  void JingleGrid::save_jingles()
  {
      if (m_jingle_filename.isEmpty()){
          m_jingle_filename = QFileDialog::getSaveFileName(this, tr("Save jingles"),
                                  "", tr("Jingles (*.jgl)"));
          if (m_jingle_filename.isEmpty())
             return;
      }

      save_to_file(m_jingle_filename);
  }

  void JingleGrid::save_as()
  {
      m_jingle_filename = QFileDialog::getSaveFileName(this, tr("Save jingles as..."), "",
                                                   tr("Jingles (*.jgl)"));
      if (m_jingle_filename.isEmpty())
          return;

      save_to_file(m_jingle_filename);
  }


  void JingleGrid::save_to_file(const QString filename)
  {
      QJsonArray jingles;

      for(auto page : std::views::iota(0, PAGE_COUNT)) {
          for(auto row: std::views::iota(0, GRID_ROWS)) {
              for (auto col : std::views::iota(0, GRID_COLS)) {

                  const auto& jingle = m_grid_cells[page][row][col];

                  if (jingle->is_empty())
                      continue;

                  auto jingle_json = jingle_to_json(jingle);
                  jingles.push_back(jingle_json);

              }
          }
      }

      QJsonObject jingle;
      jingle["jingles"] = jingles;
      QJsonDocument jingle_doc(jingle);

      QFile file(filename);
      if (!file.open(QIODevice::WriteOnly)) {
          return;
      }

      file.write(jingle_doc.toJson(QJsonDocument::Indented));
  }

  void JingleGrid::print(const QString& msg)
  {
      qDebug() << msg;
  }

}
