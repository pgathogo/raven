#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
//#include <LimeReport>

#include "../../../rave/framework/baseentitybrowserdlg.h"
#include "../../../rave/security/authentication.h"
#include "../../../rave/security/structs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QMenu;
class QAction;
QT_END_NAMESPACE

class BaseEntity;
//class PlainForm;
class PostgresDatabaseManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QApplication* app,
               const StationInfo&,
               const ConnInfo&,
               QWidget* parent = nullptr);
    ~MainWindow();

    template<typename T>
    T* createSubWindow()
    {

        T* win = new T(this);
        win->setMdiArea(mdiArea);
        mdiArea->addSubWindow(win);

        /*
        for(int i=0; i<mdiArea->subWindowList().size(); ++i){
            BaseEntityBrowserDlg* basedlg = dynamic_cast<BaseEntityBrowserDlg*>(
                        mdiArea->subWindowList().at(i));
            if (basedlg->typeID() == T::TYPEID){
                win = dynamic_cast<T*>(basedlg);
            }else{
                T* win = new T(this);
                win->setMdiArea(mdiArea);
                mdiArea->addSubWindow(win);
            }
        }
        */

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
    void showEvent(QShowEvent*);

private slots:
    void browseClients();
    void showRegister();
    void openSetupForm();
    void open_tv_programs();
    void on_schedule();
    void open_merged_browser();
    void open_setup_browser();
    void open_cue_editor();
    void test_new_booking();

    void open_playlist();
    void open_log_analysis();

    void print_comm_log();


private:
    //PlainForm* pf;
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
    static AccessMap access_map;

    StationInfo m_station_info;
    ConnInfo m_conn_info;

    //LimeReport::ReportEngine* m_report;
};
#endif // MAINWINDOW_H
