#ifndef SPOTBROWSER_H
#define SPOTBROWSER_H

#include <QDialog>
#include "../../../rave/framework/baseentitybrowserdlg.h"

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
    bool save_advert_media(SpotForm&, int, int);
    // void save_spot_media(std::shared_ptr<SpotForm>);

    void search_by_client(std::shared_ptr<Client> client);

    void set_client(std::shared_ptr<Client> client);
private:
    std::string get_extension(const std::string);

    Ui::SpotBrowser *ui;
    std::shared_ptr<Client> m_client;
};

#endif // SPOTBROWSER_H
