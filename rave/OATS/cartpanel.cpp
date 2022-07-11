#include <QStandardItemModel>
#include <QHeaderView>

#include "cartpanel.h"
#include "../audio/trackbrowser.h"
#include "../audio/trackpickerdialog.h"

namespace OATS
{
    CartPanel::CartPanel()
        :m_v_layout{nullptr}
        ,m_top_toolbar{nullptr}
        ,m_bottom_toolbar{nullptr}
        ,m_cart_a{nullptr}
        ,m_cart_b{nullptr}
        ,m_cart_c{nullptr}
    {
        m_top_toolbar = std::make_unique<PanelTopToolbar>();

        m_cart_a = std::make_unique<CartWidget>();
        m_cart_b = std::make_unique<CartWidget>();
        m_cart_c = std::make_unique<CartWidget>();

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_top_toolbar.get());
        m_v_layout->addWidget(m_cart_a.get());
        m_v_layout->addWidget(m_cart_b.get());
        m_v_layout->addWidget(m_cart_c.get());

        m_bottom_toolbar = std::make_unique<PanelBottomToolbar>();
        m_v_layout->addWidget(m_bottom_toolbar.get());

        m_v_layout->setContentsMargins(0,10,0,0);

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());
    }

    /* ---- PanelTopToolbar ----- */

    PanelTopToolbar::PanelTopToolbar()
        :m_h_layout{nullptr}
        ,m_open_btn{nullptr}
        ,m_save_btn{nullptr}
        ,m_title_lbl{nullptr}
    {
        setStyleSheet("background-color: red;");

        m_open_btn = std::make_unique<QPushButton>("Open", this);
        m_save_btn = std::make_unique<QPushButton>("Save", this);
        m_title_lbl = std::make_unique<QLabel>("Toolbar..",this);

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addWidget(m_open_btn.get());
        m_h_layout->addWidget(m_save_btn.get());
        m_h_layout->addWidget(m_title_lbl.get());

        setLayout(m_h_layout.get());

    }

    /* ----- PanelBottomToolbar ------ */

    PanelBottomToolbar::PanelBottomToolbar()
        :m_h_layout{nullptr}
        ,m_stop_all_btn{nullptr}
        ,m_clear_all_btn{nullptr}
    {
        m_stop_all_btn = std::make_unique<QPushButton>("Stop All", this);
        m_clear_all_btn = std::make_unique<QPushButton>("Clear All", this);

        m_h_layout = std::make_unique<QHBoxLayout>();
        m_h_layout->addWidget(m_stop_all_btn.get());
        m_h_layout->addWidget(m_clear_all_btn.get());

        setLayout(m_h_layout.get());
    }

    /* ----- AudioLoadWidget ----- */

    AudioLoadWidget::AudioLoadWidget()
        :m_v_layout{nullptr}
        ,m_up_btn{nullptr}
        ,m_add_btn{nullptr}
        ,m_remove_btn{nullptr}
        ,m_down_btn{nullptr}
    {
        m_up_btn = std::make_unique<QPushButton>("U", this);
        m_up_btn->setMaximumWidth(50);

        m_add_btn = std::make_unique<QPushButton>("A", this);
        m_add_btn->setMaximumWidth(50);

        connect(m_add_btn.get(), &QPushButton::clicked, this, &AudioLoadWidget::open_track_picker);

        m_remove_btn = std::make_unique<QPushButton>("R", this);
        m_remove_btn->setMaximumWidth(50);
        m_down_btn = std::make_unique<QPushButton>("D", this);
        m_down_btn->setMaximumWidth(50);

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

         create_table_view_headers();

         m_v_layout = std::make_unique<QVBoxLayout>();
         m_v_layout->addWidget(m_table_view.get());
         m_v_layout->addStretch();

        setLayout(m_v_layout.get());
     }

     void AudioViewWidget::create_table_view_headers()
     {
         m_model->setHorizontalHeaderItem(0, new QStandardItem("Title"));
         m_model->setHorizontalHeaderItem(1, new QStandardItem("Duration"));

         m_table_view->setColumnWidth(0, 300);
         m_table_view->setColumnWidth(1, 50);
     }

     void AudioViewWidget::add_audio(AUDIO::Audio* audio)
     {
         QList<QStandardItem*> columns;

         auto title = new QStandardItem(QString::fromStdString(audio->title()->value()));
         auto duration = new QStandardItem(QString::number(audio->duration()->value()));

         columns.append(title);
         columns.append(duration);

         m_model->appendRow(columns);

     }

    /* ---- AudioViewControllerWidget ---- */

    AudioViewControllerWidget::AudioViewControllerWidget()
        :m_v_layout{nullptr}
        ,m_preview_btn{nullptr}
        ,m_clear_btn{nullptr}
        ,m_group_btn{nullptr}
        ,m_next_btn{nullptr}
    {
        m_preview_btn = std::make_unique<QPushButton>("P", this);
        m_clear_btn = std::make_unique<QPushButton>("C", this);
        m_group_btn = std::make_unique<QPushButton>("G", this);
        m_next_btn = std::make_unique<QPushButton>("N", this);

        m_v_layout = std::make_unique<QVBoxLayout>();

        m_v_layout->addWidget(m_preview_btn.get());
        m_v_layout->addWidget(m_clear_btn.get());
        m_v_layout->addWidget(m_group_btn.get());
        m_v_layout->addWidget(m_next_btn.get());

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());

    }

    /* ---- AudioPlayWidget ---- */
     AudioPlayWidget::AudioPlayWidget()
         :m_v_layout{nullptr}
         ,m_timer_lbl{nullptr}
         ,m_play_btn{nullptr}
         ,m_stop_btn{nullptr}
     {
        m_timer_lbl = std::make_unique<QLabel>("00:00", this);
        m_play_btn = std::make_unique<QPushButton>("P", this);
        m_stop_btn = std::make_unique<QPushButton>("S", this);

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_timer_lbl.get());
        m_v_layout->addWidget(m_play_btn.get());
        m_v_layout->addWidget(m_stop_btn.get());

        m_v_layout->addStretch();

        setLayout(m_v_layout.get());

     }

    /* ----- CartWidget ----- */

    CartWidget::CartWidget()
        :m_h_layout{nullptr}
        ,m_audio_load_widget{nullptr}
        ,m_audio_view_widget{nullptr}
        ,m_audio_view_controller_widget{nullptr}
        ,m_play_widget{nullptr}
    {
        m_audio_load_widget = std::make_unique<AudioLoadWidget>();
        connect(m_audio_load_widget.get(), &AudioLoadWidget::cart_add_audio, this, &CartWidget::cart_add_audio);
        m_audio_view_widget = std::make_unique<AudioViewWidget>();
        m_audio_view_controller_widget = std::make_unique<AudioViewControllerWidget>();
        m_play_widget = std::make_unique<AudioPlayWidget>();

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addWidget(m_audio_load_widget.get());
        m_h_layout->addWidget(m_audio_view_widget.get());
        m_h_layout->addWidget(m_audio_view_controller_widget.get());
        m_h_layout->addWidget(m_play_widget.get());

        setLayout(m_h_layout.get());

    }

   void CartWidget::cart_add_audio(AUDIO::Audio* audio)
   {
       m_audio_view_widget->add_audio(audio);

   }

}
