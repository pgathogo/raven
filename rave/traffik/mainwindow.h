#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QMenu;
class QAction;
QT_END_NAMESPACE

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

private slots:
    void openClientBrowser();
    void newClientGroup();

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
