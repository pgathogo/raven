#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QMenu;
class QAction;
QT_END_NAMESPACE

class BaseEntity;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    template<typename T>
    T* createSubWindow()
    {
        T* win = new T(this);
        win->setMdiArea(mdiArea);
        mdiArea->addSubWindow(win);
        return win;
    }

    template<typename T1, typename T2>
    T1* createSubWindow(T2* entity)
    {
        T1* win = new T1(this, entity);
        win->setMdiArea(mdiArea);
        mdiArea->addSubWindow(win);
        return win;
    }

private slots:
    void openClientBrowser();
    void newClientGroup();
    void newGender();

private:
    Ui::MainWindow *ui;
    QMdiArea* mdiArea;

    QMenu* traffikMenu;
    QMenu* reportMenu;
    QMenu* qreportSubMenu;
    QMenu* clientRptSubMenu;
    QMenu* setupMenu;
    QMenu* helpMenu;

    QAction* clientAction;
    QAction* spotAction;
    QAction* orderAction;
    QAction* timebandAction;
    QAction* breakAction;
    QAction* commlogAction;
    QAction* viewbreakAction;
    QAction* exitAction;

    QAction* qreportAction;
    QAction* clientRptAction;

    void createActions();


};
#endif // MAINWINDOW_H
