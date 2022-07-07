
#include "cartpanel.h"

namespace OATS
{
    CartPanel::CartPanel()
        :m_v_layout{nullptr}
        ,m_top_toolbar{nullptr}
        ,m_cart_a{nullptr}
    {
        m_top_toolbar = std::make_unique<PanelTopToolbar>();
        m_cart_a = std::make_unique<CartWidget>();

        m_v_layout = std::make_unique<QVBoxLayout>();
        m_v_layout->addWidget(m_top_toolbar.get());
        m_v_layout->addWidget(m_cart_a.get());

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
        m_add_btn = std::make_unique<QPushButton>("A", this);
        m_remove_btn = std::make_unique<QPushButton>("R", this);
        m_down_btn = std::make_unique<QPushButton>("D", this);

        m_v_layout = std::make_unique<QVBoxLayout>();

        m_v_layout->addWidget(m_up_btn.get());
        m_v_layout->addWidget(m_add_btn.get());
        m_v_layout->addWidget(m_remove_btn.get());
        m_v_layout->addWidget(m_down_btn.get());

        setLayout(m_v_layout.get());
    }

    /* ----- CartWidget ----- */

    CartWidget::CartWidget()
        :m_h_layout{nullptr}
        ,m_audio_load_widget{nullptr}
    {
        m_audio_load_widget = std::make_unique<AudioLoadWidget>();

        m_h_layout = std::make_unique<QHBoxLayout>();

        m_h_layout->addWidget(m_audio_load_widget.get());
        m_h_layout->addStretch();

        setLayout(m_h_layout.get());

    }

}
