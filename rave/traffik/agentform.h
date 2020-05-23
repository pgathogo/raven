#ifndef AGENTFORM_H
#define AGENTFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class Agent;

namespace Ui {
class AgentForm;
}

class AgentForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit AgentForm(Agent* agent,
            QDialog *parent = nullptr);
    ~AgentForm() override;

    ActionResult saveRecord() override;
    void bindWidgets() override;
    std::string windowTitle() override;

private:
    Ui::AgentForm *ui;
    Agent* mAgent;
};

#endif // AGENTFORM_H
