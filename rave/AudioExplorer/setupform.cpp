#include "setupform.h"
#include "ui_setupform.h"

#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/ravensetup.h"

namespace AUDIOEXP
{

    SetupForm::SetupForm(RavenSetup* setup,
                            QDialog* parent)
        :BaseEntityDetailDlg(parent)
        ,ui(new Ui::SetupForm)
        ,m_setup{setup}

    {
        ui->setupUi(bui->baseContainer);
        setTitle(windowTitle());
        populateFormWidgets();
    }

    SetupForm::~SetupForm()
    {
        delete ui;
    }

    ActionResult SetupForm::saveRecord()
    {
        populateEntityFields();
    }

    std::string SetupForm::windowTitle()
    {
        return "Audio Explorer Setup Form";
    }

    void SetupForm::populateEntityFields()
    {
        m_setup->set_convert_to_ogg(ui->cbConvertOgg->isChecked());
    }

    void SetupForm::populateFormWidgets()
    {
        auto check_state = [](bool state){ return (state) ? Qt::Checked : Qt::Unchecked; };
        ui->cbConvertOgg->setCheckState(check_state(m_setup->convert_to_ogg()->value()));
    }
}
