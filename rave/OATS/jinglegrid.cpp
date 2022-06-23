#include <QGridLayout>
#include <QButtonGroup>
#include <QAction>
#include <QFileDialog>
#include <QDebug>

#include "jinglegrid.h"

namespace OATS
{
    Jingle::Jingle(int page_id, int col, QString title)
        :m_grid_page_id{page_id}
        ,m_col{col}
        ,m_title{title}
    {
    }

    Jingle::~Jingle()
    {
    }

    QString Jingle::title() const{
        return m_title;
    }

    int Jingle::page_id()
    {
        return m_grid_page_id;
    }

    int Jingle::col()
    {
        return m_col;
    }

    /* --------------------------------------------- */
    GridButton::GridButton(int id)
        :m_id{id}
         ,m_jingle{}
    {
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

    /* ----------------------------------------------- */

    JingleGrid::JingleGrid(QWidget *parent)
        : QWidget{parent}
        ,m_main_layout{nullptr}
        ,m_pager_layout{nullptr}
        ,m_context_menu{nullptr}
        ,m_context_action{nullptr}
    {
        m_main_layout = std::make_unique<QVBoxLayout>();

        make_jingles();
        make_toolbar(m_main_layout.get());
        make_grid_buttons(m_main_layout.get());
        make_pager(m_main_layout.get());
        make_stop_button(m_main_layout.get());
        make_context_menu();

        assign_buttons(1);

        m_main_layout->setStretch(1, 1);

        this->setLayout(m_main_layout.get());

    }

    QGridLayout* JingleGrid::layout() const
    {
        return m_grid_layout.get();
    }

    void JingleGrid::make_jingles()
    {
        auto jingle1 = std::make_unique<Jingle>(1,0, "Jingle1");
        auto jingle2 = std::make_unique<Jingle>(1,1, "Jingle2");
        auto jingle3 = std::make_unique<Jingle>(1,0, "Jingle3");
        auto jingle4 = std::make_unique<Jingle>(1,1, "Jingle4");

        auto jingle5 = std::make_unique<Jingle>(2,0, "Jingle5");
        auto jingle6 = std::make_unique<Jingle>(2,1, "Jingle6");

        m_jingles.push_back(std::move(jingle1));
        m_jingles.push_back(std::move(jingle2));
        m_jingles.push_back(std::move(jingle3));
        m_jingles.push_back(std::move(jingle4));
        m_jingles.push_back(std::move(jingle5));
        m_jingles.push_back(std::move(jingle6));
    }


    void JingleGrid::make_grid_buttons(QVBoxLayout* main_layout)
    {
       m_grid_layout = std::make_unique<QGridLayout>();

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

        m_button_widget = std::make_unique<QWidget>();
        m_button_widget->setLayout(m_grid_layout.get());

        main_layout->addWidget(m_button_widget.get());
    }

    void JingleGrid::make_context_menu()
    {
        m_context_action = std::make_unique<QAction>("Load...", this);
        m_context_menu = std::make_unique<QMenu>();
        m_context_menu->addAction(m_context_action.get());
        connect(m_context_action.get(), &QAction::triggered, this,
                [&](){ qDebug() << "Load Clicked";});
    }

    void JingleGrid::open_menu_at(int row, int col, const QPoint& pos)
    {
       qDebug() << "Row: "<< row << "Col: " << col << "Position: "<< pos;

       if (m_context_action == nullptr)
       {
        m_context_action = std::make_unique<QAction>("Load...", this);
        m_context_menu = std::make_unique<QMenu>();
        m_context_menu->addAction(m_context_action.get());
       }

        connect(m_context_action.get(), &QAction::triggered, this,
                [&](){ open_load_dialog(row, col); });
       m_context_menu->exec(m_grid_buttons[row][col]->mapToGlobal(pos));
    }

    void JingleGrid::open_load_dialog(int row, int col)
    {
        auto filename = QFileDialog::getOpenFileName(this, tr("/d/"), tr("Audio Files (*.ogg)"));
    }

    void JingleGrid::play_jingle_at(int row, int col)
    {
        auto jingle = m_grid_buttons[row][col]->jingle()->title();
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

    void JingleGrid::assign_buttons(int page_id)
    {
      clear_buttons();
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

   void JingleGrid::make_toolbar(QVBoxLayout* main_layout)
   {
       m_toolbar_layout = std::make_unique<QHBoxLayout>();

       auto open_btn = std::make_unique<QPushButton>("Open...");
       auto save_btn = std::make_unique<QPushButton>("Save");
       auto save_as_btn = std::make_unique<QPushButton>("Save As...");
       auto clear_all = std::make_unique<QPushButton>("Clear All");

       m_toolbar_layout->addStretch(1);
       m_toolbar_layout->addWidget(open_btn.get());
       m_toolbar_layout->addWidget(save_btn.get());
       m_toolbar_layout->addWidget(save_as_btn.get());
       m_toolbar_layout->addWidget(clear_all.get());

       m_tool_buttons.push_back(std::move(open_btn));
       m_tool_buttons.push_back(std::move(save_btn));
       m_tool_buttons.push_back(std::move(save_as_btn));
       m_tool_buttons.push_back(std::move(clear_all));

       m_toolbar_widget = std::make_unique<QWidget>();
       m_toolbar_widget->setLayout(m_toolbar_layout.get());

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
                    [=](){assign_buttons(i);});
            m_page_button_group->addButton(page_button.get());
            m_pager_layout->addWidget(page_button.get());
            m_page_buttons[i] = std::move(page_button);
        }

        main_layout->addLayout(m_pager_layout.get());

    }

   void JingleGrid::make_stop_button(QVBoxLayout* main_layout)
   {
       m_stop_button = std::make_unique<QPushButton>("Stop All");

       main_layout->addWidget(m_stop_button.get());

   }
}
