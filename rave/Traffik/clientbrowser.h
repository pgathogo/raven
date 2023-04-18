#ifndef CLIENTBROWSER_H
#define CLIENTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;
class Client;

namespace Ui {
class ClientBrowser;
}

class ClientBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "ClientBrowser";
    explicit ClientBrowser(QWidget *parent = nullptr);
    ~ClientBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord()  override;

    std::string typeID() override;

    void createBrandButton();
    void createSpotButton();
    void createOrderButton();


    template<typename T1, typename T2>
    void openBrowserWindow()
    {
        if (selectedRowId() < 0){
            showMessage("Select a Client to view Bla Bla Bla");
        }else{

           std::shared_ptr<BaseEntity> be = findSelectedEntity();

           T1* entity = dynamic_cast<T1*>(be.get());

           auto browser = std::make_unique<T2>(entity, this);

           browser->exec();
        }
    }


private slots:
    void openBrandBrowser();
    void openSpotBrowser();
    void openOrderBrowser();
    void selection_changed(const QItemSelection&, const QItemSelection&);
signals:
    void client_selection_changed(std::shared_ptr<BaseEntity>);

private:
    Ui::ClientBrowser *ui;
};

#endif // CLIENTBROWSER_H
