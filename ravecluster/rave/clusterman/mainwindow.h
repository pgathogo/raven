#pragma once

#include <memory>

#include <QMainWindow>

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

private slots:
    void user_browser();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<EntityDataModel> m_cluster_edm;

    QMenu* m_file_menu;
    QAction* m_setup_action;
};
