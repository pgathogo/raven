#include <chrono>

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

#include <QVBoxLayout>
#include <QPalette>

#include "jinglegrid.h"
#include "../audio/trackbrowser.h"
#include "../audio/trackpickerdialog.h"

using namespace std::chrono_literals;

namespace OATS
{
    Jingle::Jingle()
        :m_grid_page_id{-1}
        ,m_row{-1}
        ,m_col{-1}
        ,m_title{""}
        ,m_jingle_id{""}
    {
    }

    Jingle::Jingle(int page_id, int row, int col, QString title)
        :m_grid_page_id{page_id}
        ,m_row{row}
        ,m_col{col}
        ,m_title{title}
    {
        m_jingle_id = QString::number(page_id)+
               QString::number(row)+
               QString::number(col);
    }

    Jingle::~Jingle()
    {
    }

    QString Jingle::title() const{
        return m_title;
    }

    void Jingle::set_title(const QString title)
    {
        m_title = title;
    }

    int Jingle::page_id() const
    {
        return m_grid_page_id;
    }

    void Jingle::set_page_id(int id)
    {
        m_grid_page_id = id;
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


    /* ---------------- GridButton ----------------------------- */

    GridButton::GridButton(int id)
        :m_id{id}
        ,m_jingle{nullptr}
        ,m_count_down_timer{nullptr}
    {
        m_count_down_timer = std::make_unique<QTimer>(this);
        connect(m_count_down_timer.get(), &QTimer::timeout, this, &GridButton::count_down);
    }

    GridButton::GridButton(int id, Jingle* const jingle)
        :m_id{id}
         ,m_jingle{jingle}
    {
        setText(jingle->title());
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
        m_jingle = jingle;

        if (jingle == nullptr){
            setText("");
        }else{
            setText(m_jingle->title());
        }
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
        int remaining = m_jingle->track_duration() - elapsed;

        auto progress_value = (100 - round((float)remaining/m_jingle->track_duration() * 100));
        m_color_value = progress_value / 100;

        setStyleSheet(QString("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,"
                                               "stop:0 rgba(230, 103, 192, 255), stop:%1 rgba(230, 103, 192, 255),"
                                               "stop:%2 rgba(79, 179, 228, 255), stop:1 rgba(79, 179, 228, 255));")
                                                .arg(m_color_value-0.01f)
                                                .arg(m_color_value));
        if ( (progress_value >= 100) || (m_color_value >= 1.0f)){
            m_count_down_timer->stop();
        }

    }

    void GridButton::stop_countdown_timer()
    {
        m_count_down_timer->stop();
    }

    /* ----------------------------------------------- */

    JingleGrid::JingleGrid(QWidget *parent)
        : QWidget{parent}
        ,m_main_layout{nullptr}
        ,m_pager_layout{nullptr}
        ,m_context_menu{nullptr}
        ,m_load_action{nullptr}
        ,m_jingle_filename{""}
    {
        m_main_layout = std::make_unique<QVBoxLayout>();

        make_jingles();
        make_toolbar(m_main_layout.get());

        m_file_path = std::make_unique<QLabel>("Jingle fiile... ");
        m_main_layout->addWidget(m_file_path.get());

        make_grid_buttons(m_main_layout.get());

        make_stop_button(m_main_layout.get());
        make_pager(m_main_layout.get());
        //make_context_menu();

        attach_jingle_to_buttons(JINGLE_PAGE_ONE);

        m_main_layout->setStretch(2, 1);

        this->setLayout(m_main_layout.get());

    }

    QGridLayout* JingleGrid::layout() const
    {
        return m_grid_layout;
    }

    void JingleGrid::make_jingles()
    {
        auto jingle1 = std::make_unique<Jingle>(1,0,0, "Jingle1");
        jingle1->set_track_id(1);
        jingle1->set_track_path("D:/Music/Studio1/");

        auto jingle2 = std::make_unique<Jingle>(1,0,1, "Jingle2");
        jingle2->set_track_id(2);
        jingle2->set_track_path("D:/Music/Studio1/");

        auto jingle3 = std::make_unique<Jingle>(1,1,0, "Jingle3");
        jingle2->set_track_id(3);
        jingle2->set_track_path("D:/Music/Studio1/");

        auto jingle4 = std::make_unique<Jingle>(1,1,1, "Jingle4");
        jingle2->set_track_id(4);
        jingle2->set_track_path("D:/Music/Studio1/");

        auto jingle5 = std::make_unique<Jingle>(2,0,0, "Jingle5");
        jingle2->set_track_id(5);
        jingle2->set_track_path("D:/Music/Studio1/");

        auto jingle6 = std::make_unique<Jingle>(2,0,1, "Jingle6");
        jingle2->set_track_id(6);
        jingle2->set_track_path("D:/Music/Studio1/");

        m_jingles.push_back(std::move(jingle1));
        m_jingles.push_back(std::move(jingle2));
        m_jingles.push_back(std::move(jingle3));
        m_jingles.push_back(std::move(jingle4));
        m_jingles.push_back(std::move(jingle5));
        m_jingles.push_back(std::move(jingle6));
    }


    void JingleGrid::make_grid_buttons(QVBoxLayout* main_layout)
    {
       m_grid_layout = new QGridLayout(this);

       int id=1;
       m_grid_buttons.resize(GRID_ROWS);
       for(int row=0; row<= GRID_ROWS-1; ++row){
           m_grid_buttons[row].resize(GRID_COLS);
           for(int col=0; col<GRID_COLS; ++col){
               auto btn = std::make_unique<GridButton>(id++);
               btn->setContextMenuPolicy(Qt::CustomContextMenu);

               connect(btn.get(), &GridButton::customContextMenuRequested, this,
                       [=](const QPoint& pos) { open_menu_at(row, col, pos); });

               connect(btn.get(), &GridButton::clicked, this,
                       [=](){ play_jingle_at(row, col); });

               m_grid_layout->addWidget(btn.get(), row, col, 1, 1);
               m_grid_buttons[row][col] = std::move(btn);
           }
       }

       m_grid_layout->setVerticalSpacing(0);

        m_button_widget = std::make_unique<QWidget>(this);
        m_button_widget->setLayout(m_grid_layout);

        main_layout->addWidget(m_button_widget.get());
    }

    void JingleGrid::make_context_menu()
    {
        /*
        m_load_action = std::make_unique<QAction>("Load...", this);
        connect(m_load_action.get(), &QAction::triggered, this,
                [&](){ qDebug() << "Load Clicked";});

        m_clear_action = std::make_unique<QAction>("Clear", this);
        connect(m_clear_action.get(), &QAction::triggered, this,
                [](){});

        m_context_menu = std::make_unique<QMenu>();
        m_context_menu->addAction(m_load_action.get());
        m_context_menu->addAction(m_clear_action.get());
        */
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

        m_context_menu->exec(m_grid_buttons[row][col]->mapToGlobal(pos));
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

        m_current_grid_cell.row = row;
        m_current_grid_cell.col = col;

        m_track_picker_dialog = std::make_unique<AUDIO::TrackPickerDialog>();
        connect(m_track_picker_dialog.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &JingleGrid::selected_audio);
        m_track_picker_dialog->exec();

        m_load_action = nullptr;
        //p_animation->stop();
    }

    void JingleGrid::clear_jingle(int row, int col)
    {
        m_grid_buttons[row][col]->set_jingle(nullptr);
    }

   void JingleGrid::selected_audio(AUDIO::Audio* audio)
   {
       int row = m_current_grid_cell.row;
       int col = m_current_grid_cell.col;

        if (m_grid_buttons[row][col]->jingle() == nullptr){
            auto jingle = std::make_unique<Jingle>(current_page(), row, col, QString::fromStdString(audio->title()->value()));
            jingle->set_track_path(QString::fromStdString(audio->audio_lib_path()->value()));
            jingle->set_track_duration(audio->duration()->value());

            m_grid_buttons[row][col]->set_jingle(jingle.get());
            m_jingles.push_back(std::move(jingle));
            return;
        }

        auto jingle_id =QString::number(current_page())+
                        QString::number(row)+
                        QString::number(col);

        auto it = std::find_if(m_jingles.begin(), m_jingles.end(),
                              FindJingle(jingle_id));

       if(it != m_jingles.end()){
            (*it)->set_title(QString::fromStdString(audio->title()->value()));
            (*it)->set_track_path(QString::fromStdString(audio->audio_lib_path()->value()));
            (*it)->set_track_duration(audio->duration()->value());
            m_grid_buttons[row][col]->set_jingle((*it).get());
       }

   }

    void JingleGrid::play_jingle_at(int row, int col)
    {
        if (m_grid_buttons[row][col]->jingle() == nullptr)
            return;

        auto jingle = m_grid_buttons[row][col]->jingle()->title();

        qDebug() << jingle;
        qDebug() << m_grid_buttons[row][col]->jingle()->track_path();
        qDebug() << m_grid_buttons[row][col]->jingle()->track_duration();

        m_grid_buttons[row][col]->start_countdown_timer();

        emit play_jingle(jingle);
    }

    void JingleGrid::clear_buttons()
    {
        for(int row=0; row<= GRID_ROWS-1; ++row){
            for (int col=0; col<GRID_COLS; ++col){
                m_grid_buttons[row][col]->set_jingle(nullptr);
            }
        }
    }

    void JingleGrid::attach_jingle_to_buttons(int page_id)
    {
      clear_page(page_id);
      int row=0;
      int col=0;
      for(auto const& jingle: m_jingles)
      {
        if (row > GRID_ROWS)
            break;

        if (jingle->page_id() == page_id){
            m_grid_buttons[row][col]->set_jingle(jingle.get());
            ++col;
            if (col >= GRID_COLS){
                col = 0;
                ++row;
            }
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
           }
       }
   }

   void JingleGrid::make_toolbar(QVBoxLayout* main_layout)
   {
       m_toolbar_layout = new QHBoxLayout();

       open_btn = std::make_unique<QPushButton>("Open...");
       save_btn = std::make_unique<QPushButton>("Save");
       save_as_btn = std::make_unique<QPushButton>("Save As...");
       clear_all_btn = std::make_unique<QPushButton>("Clear All");

       connect(open_btn.get(), &QPushButton::clicked, this, &JingleGrid::get_jingles);
       connect(save_btn.get(), &QPushButton::clicked, this, &JingleGrid::save_jingles);
       connect(save_as_btn.get(), &QPushButton::clicked, this, &JingleGrid::save_as);
       connect(clear_all_btn.get(), &QPushButton::clicked, this, &JingleGrid::clear_all);

       m_toolbar_layout->addWidget(open_btn.get());
       m_toolbar_layout->addWidget(save_btn.get());
       m_toolbar_layout->addWidget(save_as_btn.get());
       m_toolbar_layout->addWidget(clear_all_btn.get());
       m_toolbar_layout->addStretch(1);

       m_tool_buttons.push_back(std::move(open_btn));
       m_tool_buttons.push_back(std::move(save_btn));
       m_tool_buttons.push_back(std::move(save_as_btn));
       m_tool_buttons.push_back(std::move(clear_all_btn));

       m_toolbar_widget = std::make_unique<QWidget>(this);

       m_toolbar_widget->setLayout(m_toolbar_layout);

       main_layout->addWidget(m_toolbar_widget.get());
   }

    void JingleGrid::make_pager(QVBoxLayout* main_layout)
    {
        m_pager_layout = std::make_unique<QHBoxLayout>();
        m_page_button_group = std::make_unique<QButtonGroup>();

        for(int i=1; i < PAGE_COUNT; ++i){
            auto page_button = std::make_unique<QPushButton>(QString::number(i));
            page_button->setCheckable(true);
            connect(page_button.get(), &QPushButton::clicked, this,
                    [=](){attach_jingle_to_buttons(i); set_current_page(i);});
            m_page_button_group->addButton(page_button.get());
            m_pager_layout->addWidget(page_button.get());
            m_page_buttons[i] = std::move(page_button);
        }

        m_page_buttons[1]->setChecked(true);

        main_layout->addLayout(m_pager_layout.get());

    }

    int JingleGrid::current_page()
    {
        return m_current_page;
    }

    void JingleGrid::set_current_page(int page)
    {
        m_current_page = page;
    }

   void JingleGrid::make_stop_button(QVBoxLayout* main_layout)
   {
       m_stop_button = std::make_unique<QPushButton>("Stop All");

       main_layout->addWidget(m_stop_button.get());

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

      attach_jingle_to_buttons(m_current_page);
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

      m_jingles.clear();

      for(auto&& jingle_json : jingles){
         std::unique_ptr<Jingle> jingle = std::make_unique<Jingle>();
         QJsonObject obj = jingle_json.toObject();

         jingle->set_jingle_id(obj["jingle_id"].toString());

         jingle->set_page_id(obj["page_id"].toInt());
         jingle->set_row(obj["row"].toInt());
         jingle->set_col(obj["col"].toInt());
         jingle->set_title(obj["title"].toString());
         jingle->set_track_id(obj["track_id"].toInt());
         jingle->set_track_path(obj["track_path"].toString());
         jingle->set_track_duration(obj["duration"].toInt());

         m_jingles.push_back(std::move(jingle));
      }

  }

  QJsonObject JingleGrid::jingle_to_json(std::unique_ptr<Jingle> const& jingle)
  {
     QJsonObject json;

     json["jingle_id"] = jingle->jingle_id();
     json["page_id"] = jingle->page_id();
     json["row"] = jingle->row();
     json["col"] = jingle->col();
     json["title"] = jingle->title();
     json["track_id"] = jingle->track_id();
     json["track_path"] = jingle->track_path();
     json["duration"] = jingle->track_duration();

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
     for (const auto& jingle : m_jingles) {
         auto jingle_json = jingle_to_json(jingle);
         jingles.push_back(jingle_json);
     }

     QJsonObject jingle;
     jingle["jingles"] = jingles;
     QJsonDocument jingle_doc(jingle);

     QFile file(filename);
     if (!file.open(QIODevice::WriteOnly)){
        return;
     }

     file.write(jingle_doc.toJson(QJsonDocument::Indented));
  }

}
