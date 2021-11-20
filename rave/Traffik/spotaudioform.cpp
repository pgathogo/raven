#include "spotaudioform.h"
#include "ui_spotaudioform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../audio/audio.h"

SpotAudioForm::SpotAudioForm(
        TRAFFIK::SpotAudio* spot_audio, QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::SpotAudioForm),
    m_spot_audio{spot_audio}

{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateFormWidgets();
}

SpotAudioForm::~SpotAudioForm()
{
    delete ui;
}

ActionResult SpotAudioForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = m_spot_audio->validate();
    return ar;
}

std::string SpotAudioForm::windowTitle()
{
    return "Spot Audio";
}

void SpotAudioForm::populateEntityFields()
{
//    if (m_spot_audio->audio()->currentEntity() == nullptr)
//        m_spot_audio->make_audio();
    int COMM_FOLDER_ID = 1;
    int GENRE_ID = 1;
    int MOOD_ID = 1;
    int ARTIST_ID = 1;
    m_spot_audio->set_title(ui->edtTitle->text().toStdString());
    m_spot_audio->set_short_desc(ui->edtDesc->text().toStdString());
    m_spot_audio->set_notes(ui->edtNotes->toPlainText().toStdString());
    m_spot_audio->set_audio_type("COMM");
    m_spot_audio->set_folder(COMM_FOLDER_ID);
    m_spot_audio->set_genre(GENRE_ID);
    m_spot_audio->set_mood(MOOD_ID);
    m_spot_audio->set_artist(ARTIST_ID);
    m_spot_audio->set_weight(ui->sbWeight->value());
    m_spot_audio->set_seq_no(ui->sbSequence->value());
    auto dt = QDate::currentDate();
    m_spot_audio->set_audio_year(dt.year());

}

void SpotAudioForm::populateFormWidgets()
{
//    auto audio = dynamic_cast<AUDIO::Audio*>(m_spot_audio->audio()->currentEntity());

    ui->edtTitle->setText(stoq(m_spot_audio->title()->value()));
    ui->sbDuration->setValue(m_spot_audio->duration()->value());
    ui->edtDesc->setText(stoq(m_spot_audio->short_desc()->value()));
    ui->edtNotes->setText(stoq(m_spot_audio->notes()->value()));

    ui->sbSequence->setValue(m_spot_audio->seqNo()->value());
    ui->sbWeight->setValue(m_spot_audio->weight()->value());

}
