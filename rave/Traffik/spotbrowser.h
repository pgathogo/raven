#ifndef SPOTBROWSER_H
#define SPOTBROWSER_H

#include <QDialog>
#include "../framework/baseentitybrowserdlg.h"
#include "../framework/Logger.h"

class QApplication;

class Client;
class SpotForm;

namespace Ui {
class SpotBrowser;
}


class SpotBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit SpotBrowser(std::shared_ptr<Client> client, QWidget *parent=nullptr);
    ~SpotBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void searchRecord() override;

    bool okay_to_delete(std::shared_ptr<BaseEntity>) override;

    void save_voice_overs(const SpotForm&);
    void save_type_exclusions(const SpotForm&);
    void save_spot_audio(const SpotForm&);

    void search_by_client(std::shared_ptr<Client> client);

    void set_client(std::shared_ptr<Client> client);

    void log_info(const QString msg) { Logger::info("SpotBrowser", msg);}
    void log_error(const QString msg) { Logger::error("SpotBrowser", msg);}
private:
    Ui::SpotBrowser *ui;
    std::shared_ptr<Client> m_client;
};

#endif // SPOTBROWSER_H
