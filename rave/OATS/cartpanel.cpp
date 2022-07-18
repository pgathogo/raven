#include <algorithm>
#include <chrono>

#include <QStandardItemModel>
#include <QHeaderView>
#include <QTimer>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>

#include "cartpanel.h"
#include "../audio/trackbrowser.h"
#include "../audio/trackpickerdialog.h"
#include "../audio/audiotool.h"
#include "../audio/audiofile.h"

using namespace std::chrono_literals;

namespace OATS
{
    CartItem::CartItem()
        :m_track_id{-1}
        ,m_title{""}
        ,m_track_path{""}
        ,m_track_fullname{""}
        ,m_duration{0.0}
        ,m_cart_id{-1}
    {
    }
    void CartItem::set_track_id(int id)
    {
        m_track_id = id;
    }

   int CartItem::track_id()
   {
       return m_track_id;
   }
   void CartItem::set_track_title(const QString title)
   {
       m_title = title;
   }
   QString CartItem::track_title()
   {
       return m_title;
   }

   void CartItem::set_track_path(const QString path)
   {
       m_track_path =  path;

   }

   QString CartItem::track_path()
   {
       return m_track_path;

   }

   void CartItem::set_track_fullname(const QString name)
   {
       m_track_fullname = name;

   }

   QString CartItem::track_fullname()
   {
       return m_track_fullname;
   }

   void CartItem::set_track_duration(double dur)
   {
       m_duration = dur;
   }

   double CartItem::track_duration()
   {
       return m_duration;
   }

   void CartItem::set_cart_id(int t_id)
   {
       m_cart_id = t_id;
   }

   int CartItem::cart_id()
   {
       return m_cart_id;
   }

    CartPanel::CartPanel()
        :m_v_layout{nullptr}
        ,m_top_toolbar{nullptr}
        ,m_bottom_toolbar{nullptr}
        ,m_cart_a{nullptr}
        ,m_cart_b{nullptr}
        ,m_cart_c{nullptr}
    {
        m_top_toolbar = std::make_unique<PanelTopToolbar>();
        connect(m_top_toolbar.get(), &PanelTopToolbar::save_cart_items, this, &CartPanel::save_cart_items);
        connect(m_top_toolbar.get(), &PanelTopToolbar::read_cart_items, this, &CartPanel::read_cart_items);

        m_cart_a = std::make_unique<CartWidget>(1);
        m_cart_b = std::make_unique<CartWidget>(2);
        m_cart_c = std::make_unique<CartWidget>(3);

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_top_toolbar.get());
        m_v_layout->addWidget(m_cart_a.get());
        m_v_layout->addWidget(m_cart_b.get());
        m_v_layout->addWidget(m_cart_c.get());

        m_bottom_toolbar = std::make_unique<PanelBottomToolbar>();
        connect(m_bottom_toolbar.get(), &PanelBottomToolbar::stop_all, this, &CartPanel::stop_all);
        connect(m_bottom_toolbar.get(), &PanelBottomToolbar::clear_all, this, &CartPanel::clear_all);

        m_v_layout->addWidget(m_bottom_toolbar.get());

        m_v_layout->setContentsMargins(0,10,0,0);

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());
    }

   void CartPanel::save_cart_items(QString filename)
   {
       QJsonObject cart;
       cart["cartA"] = m_cart_a->get_cart_items();
       cart["cartB"] = m_cart_b->get_cart_items();
       cart["cartC"] = m_cart_c->get_cart_items();

       QJsonDocument cart_doc(cart);

       QFile file(filename);
       if (!file.open(QIODevice::WriteOnly)){
           return;
       }

       file.write(cart_doc.toJson(QJsonDocument::Indented));
   }

    void CartPanel::read_cart_items(QByteArray& data)
    {
        QJsonDocument doc_data(QJsonDocument::fromJson(data));
        QJsonObject cart_json = doc_data.object();

        QJsonArray json_cart_items_a = cart_json["cartA"].toArray();
        QJsonArray json_cart_items_b = cart_json["cartB"].toArray();
        QJsonArray json_cart_items_c = cart_json["cartC"].toArray();

        m_cart_a->read_cart_items(json_cart_items_a);
        m_cart_b->read_cart_items(json_cart_items_b);
        m_cart_c->read_cart_items(json_cart_items_c);

    }

    void CartPanel::stop_all()
    {
        m_cart_a->stop_all();
        m_cart_b->stop_all();
        m_cart_c->stop_all();
    }

    void CartPanel::clear_all()
    {
        m_cart_a->clear_all();
        m_cart_b->clear_all();
        m_cart_c->clear_all();
    }

    /* ---- PanelTopToolbar ----- */

    PanelTopToolbar::PanelTopToolbar()
        :m_h_layout{nullptr}
        ,m_open_btn{nullptr}
        ,m_save_btn{nullptr}
        ,m_title_lbl{nullptr}
        ,m_cart_filename{""}
    {
        setStyleSheet("background-color: red;");

        m_open_btn = std::make_unique<QPushButton>("Open", this);
        connect(m_open_btn.get(), &QPushButton::clicked, this, &PanelTopToolbar::open_cart_file);

        m_save_btn = std::make_unique<QPushButton>("Save", this);
        connect(m_save_btn.get(), &QPushButton::clicked, this, &PanelTopToolbar::save_cart);

        m_title_lbl = std::make_unique<QLabel>("Toolbar..",this);

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addWidget(m_open_btn.get());
        m_h_layout->addWidget(m_save_btn.get());
        m_h_layout->addWidget(m_title_lbl.get());

        setLayout(m_h_layout.get());
    }

    void PanelTopToolbar::save_cart()
    {
        if (m_cart_filename.isEmpty()){
            m_cart_filename = QFileDialog::getSaveFileName(this, tr("Save Cart Items"),
                                                           "", tr("Cart File (*.cat"));
            if (m_cart_filename.isEmpty())
                return;
        }

        emit save_cart_items(m_cart_filename);
    }

    void PanelTopToolbar::open_cart_file()
    {
        m_cart_filename = QFileDialog::getOpenFileName(this,
                                                       tr("Open Cart file"),
                                                       "",
                                                       tr("Cart File (*.cat);; All (*.*)"));
        if (m_cart_filename.isEmpty())
            return;

        m_title_lbl->setText(m_cart_filename);

        QFile cart_file(m_cart_filename);
        if (!cart_file.open(QIODevice::ReadOnly)){
            qWarning("Couldn't open cart file");
            return;
        }

        QByteArray data = cart_file.readAll();

        emit read_cart_items(data);
    }



    /* ----- PanelBottomToolbar ------ */

    PanelBottomToolbar::PanelBottomToolbar()
        :m_h_layout{nullptr}
        ,m_stop_all_btn{nullptr}
        ,m_clear_all_btn{nullptr}
    {
        m_stop_all_btn = std::make_unique<QPushButton>("Stop All", this);
        connect(m_stop_all_btn.get(), &QPushButton::clicked, this, &PanelBottomToolbar::stop_all_clicked);

        m_clear_all_btn = std::make_unique<QPushButton>("Clear All", this);
        connect(m_clear_all_btn.get(), &QPushButton::clicked, this, &PanelBottomToolbar::clear_all_clicked);

        m_h_layout = std::make_unique<QHBoxLayout>();
        m_h_layout->addWidget(m_stop_all_btn.get());
        m_h_layout->addWidget(m_clear_all_btn.get());

        setLayout(m_h_layout.get());
    }

    void PanelBottomToolbar::stop_all_clicked()
    {
        emit stop_all();
    }
    void PanelBottomToolbar::clear_all_clicked()
    {
        emit clear_all();
    }

    /* ----- AudioLoadWidget ----- */

    AudioLoadWidget::AudioLoadWidget()
        :m_v_layout{nullptr}
        ,m_up_btn{nullptr}
        ,m_add_btn{nullptr}
        ,m_remove_btn{nullptr}
        ,m_down_btn{nullptr}
    {
        m_up_btn = std::make_unique<QPushButton>("UP", this);
        m_up_btn->setMaximumWidth(50);
        connect(m_up_btn.get(), &QPushButton::clicked, this, &AudioLoadWidget::move_item_up);

        m_add_btn = std::make_unique<QPushButton>("A", this);
        m_add_btn->setMaximumWidth(50);
        connect(m_add_btn.get(), &QPushButton::clicked, this, &AudioLoadWidget::open_track_picker);

        m_remove_btn = std::make_unique<QPushButton>("R", this);
        m_remove_btn->setMaximumWidth(50);
        connect(m_remove_btn.get(), &QPushButton::clicked, this, &AudioLoadWidget::remove_item);

        m_down_btn = std::make_unique<QPushButton>("D", this);
        m_down_btn->setMaximumWidth(50);
        connect(m_down_btn.get(), &QPushButton::clicked, this, &AudioLoadWidget::move_item_down);

        m_v_layout = std::make_unique<QVBoxLayout>();

        m_v_layout->addWidget(m_up_btn.get());
        m_v_layout->addWidget(m_add_btn.get());
        m_v_layout->addWidget(m_remove_btn.get());
        m_v_layout->addWidget(m_down_btn.get());

        m_v_layout->setContentsMargins(0,0,10,0);

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());
    }

    void AudioLoadWidget::open_track_picker()
    {
        m_track_picker_dialog = std::make_unique<AUDIO::TrackPickerDialog>();
        connect(m_track_picker_dialog.get(), &AUDIO::TrackPickerDialog::selected_audio, this, &AudioLoadWidget::selected_audio );
        m_track_picker_dialog->exec();
    }

    void AudioLoadWidget::selected_audio(AUDIO::Audio* audio)
    {
        emit cart_add_audio(audio);

    }


    /* ---- AudioViewWidget ----- */

     AudioViewWidget::AudioViewWidget()
         :m_v_layout{nullptr}
         , m_table_view{nullptr}
         ,m_model{nullptr}
     {
         m_table_view = std::make_unique<QTableView>();
         m_table_view->setMaximumHeight(150);

         m_model = std::make_unique<QStandardItemModel>(this);

         m_table_view->setModel(m_model.get());

         m_table_view->horizontalHeader()->setStretchLastSection(true);
         m_table_view->verticalHeader()->setVisible(false);
         m_table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
         m_table_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

         connect(m_table_view.get(), &QTableView::clicked, this, &AudioViewWidget::table_view_clicked);

         create_table_view_headers();

         m_v_layout = std::make_unique<QVBoxLayout>();
         m_v_layout->addWidget(m_table_view.get());
         m_v_layout->addStretch();

        setLayout(m_v_layout.get());
     }

     void AudioViewWidget::table_view_clicked(const QModelIndex& index)
     {
        int id = index.data(Qt::UserRole).toInt();
        auto cart_item = get_selected_cart_item(id);
        if (cart_item == nullptr)
            return;
     }

     void AudioViewWidget::clear_table()
     {
         m_model->clear();
         create_table_view_headers();
     }

     void AudioViewWidget::select_all_items()
     {
         m_table_view->selectAll();
     }

     void AudioViewWidget::diselect_all_items()
     {
         m_table_view->clearSelection();
     }

     void AudioViewWidget::create_table_view_headers()
     {
         m_model->setHorizontalHeaderItem(0, new QStandardItem("Title"));
         m_model->setHorizontalHeaderItem(1, new QStandardItem("Duration"));

         m_table_view->setColumnWidth(0, 280);
         m_table_view->setColumnWidth(1, 80);
     }

     void AudioViewWidget::add_audio(AUDIO::Audio* audio, int cart_id)
     {
         AUDIO::AudioTool at;

         auto cart_item = std::make_unique<CartItem>();
         cart_item->set_track_id(audio->id());
         cart_item->set_track_title(QString::fromStdString(audio->title()->value()));
         cart_item->set_track_path(QString::fromStdString(audio->audio_lib_path()->value()));

         cart_item->set_track_fullname(
                     cart_item->track_path()+
                     QString::fromStdString(at.make_audio_filename(cart_item->track_id())+".ogg"));

         cart_item->set_track_duration(audio->duration()->value());
         cart_item->set_cart_id(cart_id);

         add_to_table_view(cart_item.get());

         /*
         auto title = new QStandardItem(QString::fromStdString(audio->title()->value()));
         title->setData(audio->id(), Qt::UserRole);
         auto duration = new QStandardItem(at.format_time(audio->duration()->value()));

         QList<QStandardItem*> columns;
         columns.append(title);
         columns.append(duration);
         m_model->appendRow(columns);
         */

         m_cart_items.push_back(std::move(cart_item));

     }

     void AudioViewWidget::move_selected_item_up()
     {
         move_selected_item(AudioViewWidget::MoveDirection::Up);
     }

     void AudioViewWidget::move_selected_item_down()
     {
         move_selected_item(AudioViewWidget::MoveDirection::Down);
     }

     void AudioViewWidget::move_selected_item(MoveDirection dr)
     {
         auto indexes = m_table_view->selectionModel()->selectedRows(0);
         if (indexes.size() == 0)
             return;

         int cart_item_id = indexes[0].data(Qt::UserRole).toInt();

         auto it = std::find_if(m_cart_items.begin(), m_cart_items.end(),
                                FindCartItem(cart_item_id));

         if (it == m_cart_items.end())
             return;

         QList<QStandardItem*> columns;
         AUDIO::AudioTool at;

         auto title = new QStandardItem((*it)->track_title());
         title->setData((*it)->track_id(), Qt::UserRole);

         auto duration = new QStandardItem(at.format_time((*it)->track_duration()));
         columns.append(title);
         columns.append(duration);

         int row = m_table_view->currentIndex().row();
         int new_row = -1;

         switch(dr)
         {
           case MoveDirection::Up:
           {
             if (row == 0)
                 return;
             new_row = (row > 0) ? row - 1: row;
             break;
           }

           case MoveDirection::Down:
           {
             if (row == m_table_view->model()->rowCount()-1)
                 return;

             new_row = row+1;
             break;
           }

         }

         m_model->removeRows(row, 1);
         m_model->insertRow(new_row, columns);
     }

     int AudioViewWidget::get_cart_id()
     {
         auto indexes = m_table_view->selectionModel()->selectedRows(0);
         if (indexes.size() == 0)
             return -1;
         int cart_item_id = indexes[0].data(Qt::UserRole).toInt();
         return cart_item_id;
     }

     std::vector<int> AudioViewWidget::get_selected_cart_ids()
     {
         std::vector<int> ids;

         auto indexes = m_table_view->selectionModel()->selectedRows(0);
         for (auto index : indexes){
             int id = index.data(Qt::UserRole).toInt();
             ids.push_back(id);
         }

         return ids;
     }

     CartItems AudioViewWidget::get_selected_cart_items()
     {
        CartItems items;

        for (auto& id : get_selected_cart_ids()) {
            auto it = std::find_if(m_cart_items.begin(), m_cart_items.end(),
                                   FindCartItem(id));
            if (it != m_cart_items.end()){
                items.push_back((*it).get());
            }
        }

        return items;
     }

     double AudioViewWidget::get_selected_items_duration(CartItems cart_items)
     {
         double duration = 0;
         for (auto item : cart_items){
            duration += item->track_duration();
         }

         return duration;

     }

     void AudioViewWidget::remove_selected_item()
     {
        int cart_item_id = get_cart_id();
        if (cart_item_id == -1)
            return;

        auto it = std::find_if(m_cart_items.begin(), m_cart_items.end(),
                               FindCartItem(cart_item_id));
        if (it == m_cart_items.end())
            return;

       m_cart_items.erase(it);

       int row = m_table_view->currentIndex().row();
       m_model->removeRows(row, 1);

     }


    CartItem* AudioViewWidget::get_selected_cart_item(int cart_item_id)
    {
        auto it = std::find_if(m_cart_items.begin(), m_cart_items.end(),
                               FindCartItem(cart_item_id));

        if (it == m_cart_items.end())
            return nullptr;

        return (*it).get();
    }

    QJsonArray AudioViewWidget::get_cart_items()
    {
        QJsonArray cart_items;
        for (const auto& cart_item : m_cart_items)
        {
            auto json_item = cart_item_to_json(cart_item);
            cart_items.push_back(json_item);
        }

        return cart_items;

        /*
        QJsonObject cart;
        cart["carts"] = cart_items;
        QJsonDocument cart_doc(cart);

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)){
            return;
        }
        file.write(cart_doc.toJson(QJsonDocument::Indented));
        */

    }

    QJsonObject AudioViewWidget::cart_item_to_json(std::unique_ptr<CartItem> const& cart_item)
    {
        QJsonObject json_object;

        json_object["cart_id"] = cart_item->cart_id();
        json_object["track_id"] = cart_item->track_id();
        json_object["title"] = cart_item->track_title();
        json_object["track_path"] = cart_item->track_path();
        json_object["track_fullname"] = cart_item->track_fullname();
        json_object["duration"] = cart_item->track_duration();

        return json_object;
    }

    void AudioViewWidget::add_to_table_view(CartItem* ci)
    {
         AUDIO::AudioTool at;

         auto title = new QStandardItem(ci->track_title());
         title->setData(ci->track_id(), Qt::UserRole);
         auto duration = new QStandardItem(at.format_time(ci->track_duration()));

         QList<QStandardItem*> columns;
         columns.append(title);
         columns.append(duration);
         m_model->appendRow(columns);
    }

    void AudioViewWidget::read_cart_items(QJsonArray& data)
    {
        for (auto&& json_cart_item: data){
            std::unique_ptr<CartItem> cart_item = std::make_unique<CartItem>();
            QJsonObject obj = json_cart_item.toObject();

            cart_item->set_cart_id(obj["cart_id"].toInt());
            cart_item->set_track_id(obj["track_id"].toInt());
            cart_item->set_track_title(obj["title"].toString());
            cart_item->set_track_path(obj["track_path"].toString());
            cart_item->set_track_fullname(obj["track_fullname"].toString());
            cart_item->set_track_duration(obj["duration"].toDouble());

            add_to_table_view(cart_item.get());

            m_cart_items.push_back(std::move(cart_item));
        }

    }

    /* ---- AudioViewControllerWidget ---- */

    AudioViewControllerWidget::AudioViewControllerWidget()
        :m_v_layout{nullptr}
        ,m_preview_btn{nullptr}
        ,m_clear_btn{nullptr}
        ,m_group_btn{nullptr}
        ,m_next_btn{nullptr}
    {
        m_preview_btn = std::make_unique<QPushButton>("Preview", this);
        m_clear_btn = std::make_unique<QPushButton>("Clear", this);
        connect(m_clear_btn.get(), &QPushButton::clicked, this, &AudioViewControllerWidget::emit_clear_items);

        m_group_btn = std::make_unique<QPushButton>("Group", this);
        m_group_btn->setCheckable(true);
        connect(m_group_btn.get(), &QPushButton::clicked, this, &AudioViewControllerWidget::emit_group_clicked);

        m_next_btn = std::make_unique<QPushButton>("Next", this);

        m_v_layout = std::make_unique<QVBoxLayout>();

        m_v_layout->addWidget(m_preview_btn.get());
        m_v_layout->addWidget(m_clear_btn.get());
        m_v_layout->addWidget(m_group_btn.get());
        m_v_layout->addWidget(m_next_btn.get());

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());

    }

    void AudioViewControllerWidget::emit_clear_items()
    {
        emit clear_items();
    }

    void AudioViewControllerWidget::emit_group_clicked()
    {
        if (m_group_btn->isChecked())
            emit select_all();
        else
            emit diselect_all();
    }

    void AudioViewControllerWidget::clear_all()
    {
        emit clear_items();
    }

    /* ---- AudioPlayWidget ---- */
     CartPlayerWidget::CartPlayerWidget()
         :m_v_layout{nullptr}
         ,m_timer_lbl{nullptr}
         ,m_play_btn{nullptr}
         ,m_stop_btn{nullptr}
         ,m_start_tick_time_stamp{0}
     {
        m_timer_lbl = std::make_unique<QLabel>("00:00", this);

        m_play_btn = std::make_unique<QPushButton>("P", this);
        connect(m_play_btn.get(), &QPushButton::clicked, this, &CartPlayerWidget::play_button_clicked);

        m_stop_btn = std::make_unique<QPushButton>("S", this);
        connect(m_stop_btn.get(), &QPushButton::clicked, this, &CartPlayerWidget::stop_play);

        m_countdown_timer = std::make_unique<QTimer>(this);
        connect(m_countdown_timer.get(), &QTimer::timeout, this, &CartPlayerWidget::count_down);

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_timer_lbl.get());
        m_v_layout->addWidget(m_play_btn.get());
        m_v_layout->addWidget(m_stop_btn.get());

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());

     }

     void CartPlayerWidget::play_button_clicked()
     {
         emit play_audio_request();
     }

     void CartPlayerWidget::stop_play()
     {
         if (m_cart_status == CartStatus::Playing)
         {
             m_audio_player->stop_play();
             m_countdown_timer->stop();
             m_cart_status = CartStatus::Waiting;
         }
     }

     void CartPlayerWidget::play_audio(CartItems cart_items)
     {

       std::vector<QString> play_items;

       double duration = 0;
       for (auto ci : cart_items){
           duration += ci->track_duration();
           play_items.push_back(ci->track_fullname());
       }

       m_start_tick_time_stamp = m_audio_tool.get_tick_count();
       auto COUNT_DOWN_INTERVAL = 100ms;
       m_countdown_timer->start(COUNT_DOWN_INTERVAL);

       set_timer_label(m_audio_tool.format_time(duration));

       m_audio_player = std::make_unique<AUDIO::AudioPlayer>(play_items);
       connect(m_audio_player.get(), &AUDIO::AudioPlayer::play_next, this, &CartPlayerWidget::play_next);
       connect(m_audio_player.get(), &AUDIO::AudioPlayer::end_of_play, this, &CartPlayerWidget::end_of_play);
       m_audio_player->play_audio();

       m_cart_status = CartStatus::Playing;

     }

     void CartPlayerWidget::play_next()
     {
         qDebug() << "Play next item ...";
     }

     void CartPlayerWidget::end_of_play()
     {
         stop_play();
     }

     void CartPlayerWidget::set_timer_label(QString text)
     {
         m_timer_lbl->setText(text);
     }

     void CartPlayerWidget::set_selected_items_duration(double duration)
     {
         m_selected_items_duration = duration;
         set_timer_label(m_audio_tool.format_time(duration));
     }

    void CartPlayerWidget::count_down()
    {
        auto trigger_ticker = m_audio_tool.get_tick_count();
        long long elapsed = trigger_ticker - m_start_tick_time_stamp;
        long long remaining = m_selected_items_duration - elapsed;

        int duration_seconds = (int)remaining / 1000;
        int ds_u1 = (int)duration_seconds / 60;
        int ds_u2 = duration_seconds % 60;

        QString duration_str = QString("%1:%2").arg(ds_u1, 2, 10, QChar('0'))
                                               .arg(ds_u2, 2, 10, QChar('0'));
        set_timer_label(duration_str);
    }

    CartStatus CartPlayerWidget::cart_status()
    {
        return m_cart_status;
    }

    /* ----- CartWidget ----- */

    CartWidget::CartWidget(int cart_id)
        :m_h_layout{nullptr}
        ,m_audio_load_widget{nullptr}
        ,m_audio_view_widget{nullptr}
        ,m_audio_view_controller_widget{nullptr}
        ,m_play_widget{nullptr}
        ,m_cart_id{cart_id}
    {
        m_audio_load_widget = std::make_unique<AudioLoadWidget>();
        connect(m_audio_load_widget.get(), &AudioLoadWidget::cart_add_audio, this, &CartWidget::cart_add_audio);
        connect(m_audio_load_widget.get(), &AudioLoadWidget::move_item_up, this, &CartWidget::move_item_up);
        connect(m_audio_load_widget.get(), &AudioLoadWidget::move_item_down, this, &CartWidget::move_item_down);
        connect(m_audio_load_widget.get(), &AudioLoadWidget::remove_item, this, &CartWidget::remove_item);

        m_audio_view_widget = std::make_unique<AudioViewWidget>();

        m_audio_view_controller_widget = std::make_unique<AudioViewControllerWidget>();
        connect(m_audio_view_controller_widget.get(), &AudioViewControllerWidget::clear_items, this, &CartWidget::clear_items_signal_handler);
        connect(m_audio_view_controller_widget.get(), &AudioViewControllerWidget::select_all, this, &CartWidget::select_all_signal_handler);
        connect(m_audio_view_controller_widget.get(), &AudioViewControllerWidget::diselect_all, this, &CartWidget::diselect_all_signal_handler);

        m_play_widget = std::make_unique<CartPlayerWidget>();
        connect(m_play_widget.get(), &CartPlayerWidget::play_audio_request, this, &CartWidget::play_audio_signal_handler);

//        m_cart_player = std::make_unique<CartPlayer>();

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addWidget(m_audio_load_widget.get());
        m_h_layout->addWidget(m_audio_view_widget.get());
        m_h_layout->addWidget(m_audio_view_controller_widget.get());
        m_h_layout->addWidget(m_play_widget.get());

        setLayout(m_h_layout.get());

    }


    QJsonArray CartWidget::get_cart_items()
    {
       return m_audio_view_widget->get_cart_items();
    }

    int CartWidget::get_cart_id()
    {
        return m_cart_id;
    }


   void CartWidget::cart_add_audio(AUDIO::Audio* audio)
   {
       m_audio_view_widget->add_audio(audio, m_cart_id);
   }

   void CartWidget::read_cart_items(QJsonArray& data)
   {
       m_audio_view_widget->read_cart_items(data);
   }

   void CartWidget::move_item_up()
   {
       m_audio_view_widget->move_selected_item_up();
   }

   void CartWidget::move_item_down()
   {
       m_audio_view_widget->move_selected_item_down();
   }

   void CartWidget::remove_item()
   {
      m_audio_view_widget->remove_selected_item();
   }

   void CartWidget::play_audio_signal_handler()
   {
      if (m_play_widget->cart_status() == CartStatus::Playing)
           return;

      CartItems cart_items = m_audio_view_widget->get_selected_cart_items();
      if (cart_items.size() == 0)
          return;

      m_play_widget->set_selected_items_duration(m_audio_view_widget->get_selected_items_duration(cart_items));
      m_play_widget->play_audio(cart_items);
   }

   void CartWidget::clear_items_signal_handler()
   {
       m_audio_view_widget->clear_table();
   }

   void CartWidget::select_all_signal_handler()
   {
       m_audio_view_widget->select_all_items();
   }

   void CartWidget::diselect_all_signal_handler()
   {
       m_audio_view_widget->diselect_all_items();
   }

   void CartWidget::stop_all()
   {
       m_play_widget->stop_play();
   }

   void CartWidget::clear_all()
   {
       m_audio_view_controller_widget->clear_all();
   }


}
