#ifndef CARTPANEL_H
#define CARTPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>

class QStandardItemModel;

namespace AUDIO
{
  class TrackPickerDialog;
  class Audio;
}

namespace OATS
{
    class PanelTopToolbar;
    class PanelBottomToolbar;
    class CartWidget;
    class AudioLoadWidget;
    class AudioViewWidget;
    class AudioViewControllerWidget;
    class AudioPlayWidget;


    class CartPanel : public QWidget
    {
        Q_OBJECT
    public:
        CartPanel();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<PanelTopToolbar> m_top_toolbar;
        std::unique_ptr<PanelBottomToolbar> m_bottom_toolbar;
        std::unique_ptr<CartWidget> m_cart_a;
        std::unique_ptr<CartWidget> m_cart_b;
        std::unique_ptr<CartWidget> m_cart_c;
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
    signals:
        void cart_add_audio(AUDIO::Audio*);
    private slots:
        void open_track_picker();
        void selected_audio(AUDIO::Audio*);
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QPushButton> m_up_btn;
        std::unique_ptr<QPushButton> m_add_btn;
        std::unique_ptr<QPushButton> m_remove_btn;
        std::unique_ptr<QPushButton> m_down_btn;

        std::unique_ptr<AUDIO::TrackPickerDialog> m_track_picker_dialog;
    };

    /* ----- AudioViewWidget ----- */
    class AudioViewWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioViewWidget();
         void create_table_view_headers();
         void add_audio(AUDIO::Audio*);
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QTableView> m_table_view;
        std::unique_ptr<QStandardItemModel> m_model;
    };

    /* ----- AudioViewControllerWidget ---- */
    class AudioViewControllerWidget : public QWidget
    {
        Q_OBJECT
    public:
         AudioViewControllerWidget();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QPushButton> m_preview_btn;
        std::unique_ptr<QPushButton> m_clear_btn;
        std::unique_ptr<QPushButton> m_group_btn;
        std::unique_ptr<QPushButton> m_next_btn;
    };

    /* ---- AudioPlayWidget ---- */

    class AudioPlayWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioPlayWidget();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QLabel> m_timer_lbl;
        std::unique_ptr<QPushButton> m_play_btn;
        std::unique_ptr<QPushButton> m_stop_btn;
    };


    /* ---- CartWidget ----- */
    class CartWidget : public QWidget
    {
        Q_OBJECT
    public:
        CartWidget();
    private slots:
        void cart_add_audio(AUDIO::Audio*);
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<AudioLoadWidget> m_audio_load_widget;
        std::unique_ptr<AudioViewWidget> m_audio_view_widget;
        std::unique_ptr<AudioViewControllerWidget> m_audio_view_controller_widget;
        std::unique_ptr<AudioPlayWidget> m_play_widget;
    };



}

#endif // CARTPANEL_H
