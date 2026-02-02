#ifndef JINGLEGRID_H
#define JINGLEGRID_H

#include <tuple>

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QMenu>

#include "../audio/audiotool.h"

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QAction;
class QJsonDocument;
class QJsonObject;
class QProgressBar;
class QTimer;

namespace AUDIO{
    class TrackPickerDialog;
    class Audio;
}

namespace OATS
{
    /* Jingle */

    class Jingle
    {
    public:
        enum class JingleStatus{Ready, Error};
      Jingle();
      Jingle(int page, int row, int col,  QString title);
      ~Jingle();
      void set_page(int);
      void set_row(int);
      void set_col(int);
      int page() const;
      int row() const;
      int col() const;
      QString jingle_id() const;
      void set_jingle_id(const QString);
      void set_jingle_status(Jingle::JingleStatus);

      void set_title(QString title);
      QString title() const;
      bool is_empty();

      /*
      void set_title(const QString);
      int track_id();
      void set_track_id(int);
      QString track_path();
      void set_track_path(QString);
      int track_duration();
      void set_track_duration(int);
      */

      void set_audio(std::shared_ptr<AUDIO::Audio>);
      std::shared_ptr<AUDIO::Audio> audio();
    private:
        QString m_jingle_id{""};
        int m_page{-1};
        int m_row{-1};
        int m_col{-1};

        Jingle::JingleStatus m_jingle_status;
        std::shared_ptr<AUDIO::Audio> m_audio;
        QString m_title;
        /*
        int m_track_id{-1};
        QString m_track_path;
        int m_track_duration;
        */

    };

    /* GridButton */

    class GridButton : public QPushButton
    {
        Q_OBJECT
    public:
        GridButton(int page, int row, int col);
        GridButton(int page, Jingle* const jingle);
        ~GridButton();
        void set_jingle(Jingle* const jingle);
        Jingle* jingle();
        void start_countdown_timer();
        void stop_countdown_timer();

        void switch_on();
        void switch_off();
        void remove_indicator();

        int page() { return m_page;   }
        void set_page(int p) { m_page = p; }
        int row()  { return m_row;  }
        int col()  { return m_col;  }

        QString button_name() { return QString("Row %1 Col %2")
                .arg(QString::number(m_row))
                .arg(QString::number(m_col)); }

        void set_pixmap(const QPixmap&);
        virtual QSize sizeHint() const override;
    protected:
        virtual void paintEvent(QPaintEvent*) override;

    private slots:
        void count_down();

    private:
        int m_page{-1};
        int m_row{-1};
        int m_col{-1};

        Jingle* m_jingle;
        std::unique_ptr<QTimer> m_count_down_timer;
        long long m_start_tick_count;
        AUDIO::AudioTool m_audio_tool;
        float m_color_value{0.0f};

        QPixmap m_pixmap;
    };


    /* JingleGrid */
    class JingleGrid : public QWidget
    {
        Q_OBJECT
    public:
        const int PAGE_COUNT = 10;
        const int GRID_ROWS  = 5; // 10
        const int GRID_COLS  = 4; // 2

        int JINGLE_PAGE_ONE = 1;

        struct GridCell{
            int page{-1};
            int row{-1};
            int col{-1};
        };

        explicit JingleGrid(QWidget *parent = nullptr);
        QGridLayout* layout() const;

        void make_toolbar(QVBoxLayout*);

        void print_grid_buttons2();

        void make_grid_cells();
        void make_jingle_view();

        void make_pager(QVBoxLayout*);
        void make_stop_button(QVBoxLayout*);

        void clear_buttons();
        void stop_all();
        void open_menu_at(int, int,  const QPoint&);
        void open_load_dialog(int, int);
        void clear_jingle(int, int);
        int current_page();
        void switch_page(int);
        QJsonObject jingle_to_json(std::unique_ptr<Jingle> const&);

        void save_to_file(const QString);
        void open_from_file(const QString);
        void clear_page(int);

        void print(const QString&);
        void print_grid_cells();

        void stop_play();

    signals:
        //void play_jingle(const QString);
        void play_jingle(Jingle*);
        void stop_all_jingles();

    private slots:
        void get_jingles();
        void save_jingles();
        void clear_all();
        void save_as();

        //void selected_audio(std::shared_ptr<AUDIO::Audio>);

        void selected_audio2(std::shared_ptr<AUDIO::Audio>);
        void close_track_picker(int);

        void grid_button_clicked(int, int);
        void load_audio();

    private:
        std::unique_ptr<QVBoxLayout> m_main_layout;
        std::unique_ptr<QHBoxLayout> m_pager_layout;

        QGridLayout* m_grid_layout;
        QHBoxLayout* m_toolbar_layout;

        std::unique_ptr<QWidget> m_button_widget;
        std::unique_ptr<QWidget> m_toolbar_widget;
        std::unique_ptr<QPushButton> m_stop_button;

        std::unique_ptr<QButtonGroup> m_page_button_group;
        std::unique_ptr<QMenu> m_context_menu;
        std::unique_ptr<QAction> m_load_action;
        std::unique_ptr<QAction> m_clear_action;

        std::vector<std::unique_ptr<Jingle>> m_jingles;

        std::vector<std::vector<std::unique_ptr<GridButton>>> m_grid_buttons;

        std::map<int, std::map<int, std::map<int, std::unique_ptr<Jingle>>>> m_grid_cells;

        std::map<int, std::unique_ptr<QPushButton>> m_page_buttons;
        std::vector<std::unique_ptr<QPushButton>> m_tool_buttons;
        GridButton* m_current_grid_button;

        std::unique_ptr<QPushButton> m_load_btn;
        std::unique_ptr<QPushButton> m_open_btn;
        std::unique_ptr<QPushButton> m_save_btn;
        std::unique_ptr<QPushButton> m_save_as_btn;
        std::unique_ptr<QPushButton> m_clear_all_btn;

        int m_current_page{1};
        std::unique_ptr<QLabel> m_file_path;
        QString m_jingle_filename;

        std::map<int, QWidget*> m_pages;

        std::unique_ptr<AUDIO::TrackPickerDialog> m_track_picker_dialog;
        bool m_is_track_picker_open{false};

        GridCell m_current_grid_cell;
        bool m_play_is_on{false};
    };

    struct FindJingle{
        FindJingle(QString id): m_id{id}{}
        bool operator()(std::unique_ptr<OATS::Jingle> const& jingle){
            return (jingle->jingle_id() == m_id);
        }
    private:
        QString m_id;
    };


}

#endif // JINGLEGRID_H
