#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <QWidget>
#include "../framework/baseentitydetaildlg.h"

class RavenSetup;

namespace AUDIOEXP
{
    namespace Ui {
        class SetupForm;
    }

    class SetupForm : public BaseEntityDetailDlg
    {
        Q_OBJECT

    public:
        explicit SetupForm(RavenSetup* setup,
                           QDialog* parent = nullptr);
        ~SetupForm();

        ActionResult saveRecord() override;
        std::string windowTitle() override;

        void populateEntityFields() override;
        void populateFormWidgets() override;

    public slots:
        void find_editor();

    private:
        Ui::SetupForm *ui;
        RavenSetup* m_setup;
    };
}

#endif // SETUPFORM_H
