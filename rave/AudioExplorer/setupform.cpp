#include <QFileDialog>

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

        connect(ui->btnEditor, &QToolButton::clicked, this, &SetupForm::find_editor);
    }

    SetupForm::~SetupForm()
    {
        delete ui;
    }

    void SetupForm::find_editor()
    {
        auto filename = QFileDialog::getOpenFileName(this,
                         tr("Find Audio Editor"),"",tr("Executable File (*.exe)"));

        if (filename != "")
            ui->edtEditor->setText(filename);

    }

    ActionResult SetupForm::saveRecord()
    {
        populateEntityFields();
        return ActionResult(ActionResultType::arSUCCESS, "");
    }

    std::string SetupForm::windowTitle()
    {
        return "Audio Explorer Setup Form";
    }

    void SetupForm::populateEntityFields()
    {
        m_setup->set_convert_to_ogg(ui->cbConvertOgg->isChecked());
        m_setup->set_editor_filepath(ui->edtEditor->text().toStdString());
    }

    void SetupForm::populateFormWidgets()
    {
        auto check_state = [](bool state){ return (state) ? Qt::Checked : Qt::Unchecked; };
        ui->cbConvertOgg->setCheckState(check_state(m_setup->convert_to_ogg()->value()));

        ui->edtEditor->setText(m_setup->editor_filepath()->to_qstring());
    }
}
