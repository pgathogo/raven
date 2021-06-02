#ifndef SPOTBROWSER_H
#define SPOTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"

class Client;
class SpotForm;

namespace Ui {
class SpotBrowser;
}

class SpotBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit SpotBrowser(Client* client, QWidget *parent=nullptr);
    ~SpotBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

    bool okay_to_delete(BaseEntity*) override;

    void save_voice_overs(const SpotForm&);
    void save_type_exclusions(const SpotForm&);
    void save_spot_audio(const SpotForm&);
private:
    Ui::SpotBrowser *ui;
    Client* m_client;
};

#endif // SPOTBROWSER_H
