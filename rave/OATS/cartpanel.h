#ifndef CARTPANEL_H
#define CARTPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

namespace OATS
{
    class PanelTopToolbar;
    class PanelBottomToolbar;
    class CartWidget;
    class AudioLoadWidget;
    class AudioViewWidget;
    class AudioViewControlWidget;
    class AudioPlayWidget;


    class CartPanel : public QWidget
    {
        Q_OBJECT
    public:
        CartPanel();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<PanelTopToolbar> m_top_toolbar;
        std::unique_ptr<CartWidget> m_cart_a;
    };

    /* ---- PanelTopToolbar ------ */
    class PanelTopToolbar : public QWidget
    {
        Q_OBJECT
    public:
        PanelTopToolbar();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<QPushButton> m_open_btn;
        std::unique_ptr<QPushButton> m_save_btn;
        std::unique_ptr<QLabel> m_title_lbl;
    };

    /* --- PanelBottomToolbar ---- */
    class PanelBottomToolbar : public QWidget
    {
        Q_OBJECT
    public:
        PanelBottomToolbar();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<QPushButton> m_stop_all_btn;
        std::unique_ptr<QPushButton> m_clear_all_btn;
    };

   /* ----- AudioLoadWidget ----- */
    class AudioLoadWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioLoadWidget();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QPushButton> m_up_btn;
        std::unique_ptr<QPushButton> m_add_btn;
        std::unique_ptr<QPushButton> m_remove_btn;
        std::unique_ptr<QPushButton> m_down_btn;
    };


    /* ---- CartWidget ----- */
    class CartWidget : public QWidget
    {
        Q_OBJECT
    public:
        CartWidget();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<AudioLoadWidget> m_audio_load_widget;
    };



}

#endif // CARTPANEL_H
