#ifndef CARTPANEL_H
#define CARTPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>
#include "../audio/editor/audioplayer.h"

class QStandardItemModel;

namespace AUDIO
{
  class TrackPickerDialog;
  class Audio;
  class AudioPlayer;
}

namespace OATS
{
    class PanelTopToolbar;
    class PanelBottomToolbar;
    class CartWidget;
    class AudioLoadWidget;
    class AudioViewWidget;
    class AudioViewControllerWidget;
    class CartPlayerWidget;
    class CartItem;

    using CartItems = std::vector<CartItem*>;

    class CartItem
    {
    public:
        enum class CartID{CartA=1, CartB, CartC};
        CartItem();
        void set_track_id(int);
        int track_id();
        void set_track_title(const QString);
        QString track_title();
        void set_track_path(const QString);
        QString track_path();
        void set_track_fullname(const QString);
        QString track_fullname();
        void set_track_duration(double);
        double track_duration();
        void set_cart_id(int);
        int cart_id();
    private:
        int m_track_id;
        QString m_title;
        QString m_track_path;
        QString m_track_fullname;
        double m_duration;
        int m_cart_id;
    };

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
        void move_item_up();
        void move_item_down();
        void remove_item();
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
        enum class MoveDirection{Up, Down};

         AudioViewWidget();
         void create_table_view_headers();
         void add_audio(AUDIO::Audio*, int);
         void move_selected_item_up();
         void move_selected_item_down();
         void move_selected_item(MoveDirection);
         void remove_selected_item();

         int get_cart_id();
         CartItem* get_selected_cart_item(int);
         double get_selected_items_duration(CartItems);

        std::vector<int> get_selected_cart_ids();
        CartItems get_selected_cart_items();

    private slots:
         void table_view_clicked(const QModelIndex&);

    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QTableView> m_table_view;
        std::unique_ptr<QStandardItemModel> m_model;
        std::vector<std::unique_ptr<CartItem>> m_cart_items;
    };

    struct FindCartItem{
        FindCartItem(int id): m_id{id}{}
        bool operator()(std::unique_ptr<OATS::CartItem> const& cart_item){
            return (cart_item->track_id() == m_id);
        }
    private:
        int m_id;
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
    class CartPlayerWidget : public QWidget
    {
        Q_OBJECT
    public:
        CartPlayerWidget();
        void play_cart_items(CartItems);
        void set_timer_label(QString);
        void set_selected_items_duration(double);

    signals:
        void play_audio();
    private slots:
        void play_button_clicked();
        void stop_play();
    private:
        std::unique_ptr<QVBoxLayout> m_v_layout;
        std::unique_ptr<QLabel> m_timer_lbl;
        std::unique_ptr<QPushButton> m_play_btn;
        std::unique_ptr<QPushButton> m_stop_btn;
        std::unique_ptr<AUDIO::AudioPlayer> m_audio_player;
        double m_selected_items_duration;
    };


    /* ---- CartWidget ----- */
    class CartWidget : public QWidget
    {
        Q_OBJECT
    public:
        CartWidget(int);
    private slots:
        void cart_add_audio(AUDIO::Audio*);
        void move_item_up();
        void move_item_down();
        void remove_item();
        int get_cart_id();
        void play_audio();
    private:
        std::unique_ptr<QHBoxLayout> m_h_layout;
        std::unique_ptr<AudioLoadWidget> m_audio_load_widget;
        std::unique_ptr<AudioViewWidget> m_audio_view_widget;
        std::unique_ptr<AudioViewControllerWidget> m_audio_view_controller_widget;
        std::unique_ptr<CartPlayerWidget> m_play_widget;
        int m_cart_id;
    };



}

#endif // CARTPANEL_H
