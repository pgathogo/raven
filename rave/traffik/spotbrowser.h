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
    explicit SpotBrowser(Client* client, QWidget *parent = nullptr);
    ~SpotBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

    bool okay_to_delete(BaseEntity* entity) override;

    void saveVoiceOvers(const SpotForm& sf);
    void saveTypeExclusions(const SpotForm& sf);
    void save_spot_audio(const SpotForm& sf);
private:
    Ui::SpotBrowser *ui;
    Client* m_client;
};

#endif // SPOTBROWSER_H
