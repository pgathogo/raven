#ifndef AGENTBROWSER_H
#define AGENTBROWSER_H

#include <QDialog>

#include "../framework/baseentitybrowserdlg.h"

class EntityDataModel;

namespace Ui {
class AgentBrowser;
}

class AgentBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "AgentBrowser";
    explicit AgentBrowser(QWidget *parent = nullptr);
    ~AgentBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    std::string typeID() override;

private:
    Ui::AgentBrowser *ui;
};

#endif // AGENTBROWSER_H
