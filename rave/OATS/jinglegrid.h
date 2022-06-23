#ifndef JINGLEGRID_H
#define JINGLEGRID_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QMenu>

class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;
class QAction;

namespace OATS
{
    /* Jingle */

    class Jingle
    {
    public:
      Jingle(int page_id, int col,  QString title);
      ~Jingle();
      QString title() const;
      int page_id();
      int col();
    private:
        int m_grid_page_id{-1};
        int m_col{-1};
        QString m_title;
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
        const int GRID_ROWS = 8;
        const int GRID_COLS = 2;

        explicit JingleGrid(QWidget *parent = nullptr);
        QGridLayout* layout() const;

        void make_jingles();
        void make_toolbar(QVBoxLayout*);
        void make_grid_buttons(QVBoxLayout*);
        void make_pager(QVBoxLayout*);
        void make_stop_button(QVBoxLayout*);

        void assign_buttons(int page_id);
        void clear_buttons();
        void play_jingle_at(int, int);
        void open_menu_at(int, int,  const QPoint&);
        void make_context_menu();
       void open_load_dialog(int, int);

    signals:
        void play_jingle(const QString);

    private:
        std::unique_ptr<QVBoxLayout> m_main_layout;
        std::unique_ptr<QGridLayout> m_grid_layout;
        std::unique_ptr<QHBoxLayout> m_pager_layout;
        std::unique_ptr<QHBoxLayout> m_toolbar_layout;
        std::unique_ptr<QWidget> m_button_widget;
        std::unique_ptr<QWidget> m_toolbar_widget;
        std::unique_ptr<QPushButton> m_stop_button;

        std::unique_ptr<QButtonGroup> m_page_button_group;
        std::unique_ptr<QMenu> m_context_menu;
        std::unique_ptr<QAction> m_context_action;

        std::vector<std::unique_ptr<Jingle>> m_jingles;
        std::vector<std::vector<std::unique_ptr<GridButton>>> m_grid_buttons;
        std::map<int, std::unique_ptr<QPushButton>> m_page_buttons;
        std::vector<std::unique_ptr<QPushButton>> m_tool_buttons;
    };



}

#endif // JINGLEGRID_H
