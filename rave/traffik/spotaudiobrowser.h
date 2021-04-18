#ifndef SPOTAUDIOBROWSER_H
#define SPOTAUDIOBROWSER_H

#include <QWidget>

#include "../framework/manytomanybrowser.h"

namespace Ui {
class SpotAudioBrowser;
}

class ManyToMany;

class SpotAudioBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    explicit SpotAudioBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~SpotAudioBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    void createImportButton();

    std::string typeID() override;

private slots:
    void import_audio();

private:
    Ui::SpotAudioBrowser* ui;
    ManyToMany* m_mtom;
};

#endif // SPOTAUDIOBROWSER_H
