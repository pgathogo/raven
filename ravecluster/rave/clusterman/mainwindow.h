#pragma once

#include <memory>

#include <QMainWindow>
#include <QMdiArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QMenu;
class QAction;

class EntityDataModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void connect_rave_db();
    void connect_cluster_db();
    void setup_menu();

    template<typename T>
    T* create_sub_window()
    {
        T* sub_win = new T(this);
        sub_win->setMdiArea(m_mdi_area);
        m_mdi_area->addSubWindow(sub_win);
        return sub_win;
    }

private slots:
    void user_browser();
    void cluster_browser();
    void cluster_manager();

private:
    Ui::MainWindow *ui;
    QMdiArea* m_mdi_area;

    std::unique_ptr<EntityDataModel> m_cluster_edm;

    QMenu* m_file_menu;
    QAction* m_setup_act;
    QAction* m_cluster_act;
};
