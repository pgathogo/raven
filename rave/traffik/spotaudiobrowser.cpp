#include <QFileDialog>

#include "spotaudiobrowser.h"
#include "ui_spotaudiobrowser.h"

#include "../framework/manytomany.h"
#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audiolib/headers/cueeditor.h"

SpotAudioBrowser::SpotAudioBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent)
        :BaseEntityBrowserDlg(parent, mtom->cloneAsUnique()),
        ui(new Ui::SpotAudioBrowser),
        m_mtom{mtom}
{
    ui->setupUi(this);
    hideAddButton();
    hideEditButton();
    hideDeleteButton();

    createImportButton();

    layout->addWidget(this);
}

SpotAudioBrowser::~SpotAudioBrowser()
{
    delete ui;
}

void SpotAudioBrowser::addRecord()
{

}

void SpotAudioBrowser::updateRecord()
{

}

void SpotAudioBrowser::deleteRecord()
{

}

void SpotAudioBrowser::createImportButton()
{
    QPushButton* btnImport = new QPushButton("Import");
    btnImport->setObjectName("btnImport");
    connect(btnImport, &QPushButton::clicked, this, &SpotAudioBrowser::import_audio);
    bui->hlExtra->addWidget(btnImport);
}

std::string SpotAudioBrowser::typeID()
{
    return "SpotAudioBrowser";
}

void SpotAudioBrowser::import_audio()
{
    auto audio_file_full_path = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), "/d/home/audio",
                                                   tr("Audio Files (*.ogg *.mp3 *.wav)"));
    if (audio_file_full_path.isEmpty())
        return;

    AudioFile audio_file(audio_file_full_path.toStdString());
    ADFRepository adf_repo;
    AudioTool audio_tool;

    CueEditor* cue_editor = new CueEditor(audio_file);
    if (cue_editor->editor() == 1)
        audio_file.set_marker(cue_editor->marker());

    delete cue_editor;
}
