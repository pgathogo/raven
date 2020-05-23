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
class PlainForm;
class PostgresDatabaseManager;

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

    template<typename T1>
    T1* createSubWindow(std::string vltype)
    {
        T1* win = new T1(vltype, this);
        win->setMdiArea(mdiArea);
        mdiArea->addSubWindow(win);
        return win;
    }

private slots:
    void openClientBrowser();
    void newClientGroup();
    void newGender();
    void plainForm();
    void newTypeExclusion();
    void newVoiceOver();
    void browseSalesPerson();
    void browseAgent();

private:
    PlainForm* pf;
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
    QAction* plainFormAction;

    PostgresDatabaseManager* mPGManager;
    void createActions();


};
#endif // MAINWINDOW_H
