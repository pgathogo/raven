#ifndef JINGLEGRID_H
#define JINGLEGRID_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QMenu>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QAction;
class QJsonDocument;
class QJsonObject;

namespace OATS
{
    /* Jingle */

    class Jingle
    {
    public:
      Jingle();
      Jingle(int page_id, int row, int col,  QString title);
      ~Jingle();
      QString title() const;
      void set_title(const QString);
      void set_page_id(int);
      void set_row(int);
      void set_col(int);
      int page_id() const;
      int row() const;
      int col() const;
      QString jingle_id() const;
      void set_jingle_id(const QString);

      int track_id();
      void set_track_id(int);
      QString track_path();
      void set_track_path(QString);
    private:
        QString m_jingle_id{""};
        int m_grid_page_id{-1};
        int m_row{-1};
        int m_col{-1};
        QString m_title;

        int m_track_id{-1};
        QString m_track_path;
    };

    /* GridButton */

    class GridButton : public QPushButton
    {
    public:
        GridButton(int id);
        GridButton(int id, Jingle* const jingle);
        ~GridButton();
        void set_jingle(Jingle* const jingle);
        Jingle* jingle();
    private:
        int m_id{-1};
        Jingle* m_jingle;
    };

    /* JingleGrid */
    class JingleGrid : public QWidget
    {
        Q_OBJECT
    public:
        const int PAGE_COUNT = 9;
        const int GRID_ROWS = 10;
        const int GRID_COLS = 2;

        int JINGLE_PAGE_ONE = 1;

        explicit JingleGrid(QWidget *parent = nullptr);
        QGridLayout* layout() const;

        void make_jingles();
        void make_toolbar(QVBoxLayout*);
        void make_grid_buttons(QVBoxLayout*);
        void make_pager(QVBoxLayout*);
        void make_stop_button(QVBoxLayout*);

        void attach_jingle_to_buttons(int page_id);
        void clear_buttons();
        void play_jingle_at(int, int);
        void open_menu_at(int, int,  const QPoint&);
        void make_context_menu();
        void open_load_dialog(int, int);
        int current_page();
        void set_current_page(int);
        QJsonObject jingle_to_json(std::unique_ptr<Jingle> const&);

        void save_to_file(const QString);
        void open_from_file(const QString);
        void clear_page(int);

    signals:
        void play_jingle(const QString);

    private slots:
        void get_jingles();
        void save_jingles();
        void clear_all();
        void save_as();

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
        std::map<int, std::unique_ptr<QPushButton>> m_page_buttons;
        std::vector<std::unique_ptr<QPushButton>> m_tool_buttons;

        std::unique_ptr<QPushButton> open_btn;
        std::unique_ptr<QPushButton> save_btn;
        std::unique_ptr<QPushButton> save_as_btn;
        std::unique_ptr<QPushButton> clear_all_btn;

        int m_current_page{1};
        std::unique_ptr<QLabel> m_file_path;
        QString m_jingle_filename;
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
