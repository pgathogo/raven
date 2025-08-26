#include <filesystem>
#include <QFileInfo>
#include "audioform.h"
#include "ui_audioform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/choicefield.h"
#include "../framework/picklistbrowser.h"
#include "../framework/ravensetup.h"

#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotool.h"

namespace fs = std::filesystem;

AudioForm::AudioForm(AUDIO::Audio* audio,
                     std::shared_ptr<RavenSetup> setup,
                     FormMode form_mode,
                     QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::AudioForm)
    ,m_audio{audio}
    ,m_setup{setup}
    ,m_artist_picker{nullptr}
    ,m_audio_tool{nullptr}
    ,m_form_mode{form_mode}
{
    m_audio_tool = std::make_unique<AUDIO::AudioTool>();

    ui->setupUi(bui->baseContainer);

    std::string title_tag = "";
    m_title_tag = (audio->id() == -1) ? "New" : "Edit";

    setTitle(windowTitle());
    setMaximumWidth(500);
    setMaximumHeight(455);

    populateFormWidgets();

    connect(ui->btnPickArtist, &QPushButton::clicked, this, &AudioForm::pick_artist);
    connect(ui->cbGenre, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioForm::genre_combo_changed);

    register_input_widget(ui->edtTitle);
    register_input_widget(ui->edtArtist);
    register_input_widget(ui->edtFolder);
    register_input_widget(ui->edtNotes);

    setup_ui();

}

void AudioForm::setup_ui()
{
    if (m_form_mode == FormMode::ReadOnly) {
        ui->btnPickArtist->setEnabled(false);
        ui->cbClass->setEnabled(false);
        ui->cbGenre->setEnabled(false);
        ui->edtTitle->setReadOnly(true);
        ui->edtArtist->setReadOnly(true);
        ui->edtFolder->setReadOnly(true);
        ui->edtFilename->setReadOnly(true);
        ui->edtDuration->setReadOnly(true);
        ui->edtStart->setReadOnly(true);
        ui->edtFadeIn->setReadOnly(true);
        ui->edtIntro->setReadOnly(true);
        ui->edtExtro->setReadOnly(true);
        ui->edtFadeOut->setReadOnly(true);
        ui->edtEnd->setReadOnly(true);
        ui->edtNotes->setReadOnly(true);
        ui->dtCreation->setReadOnly(true);

        disableSaveBtn();
    }
}

AudioForm::~AudioForm()
{
    delete ui;
}

ActionResult AudioForm::saveRecord()
{
    populateEntityFields();
    return m_audio->validate();
}

std::string AudioForm::windowTitle()
{
    return "Audio Details- "+m_title_tag+" Audio";
}

void AudioForm::populateEntityFields()
{
    m_audio->set_title(ui->edtTitle->text().toStdString());
    set_choice_field(m_audio->audio_type(), ui->cbClass);
    m_audio->set_audio_year(ui->edtYear->value());
    m_audio->set_creation_date(ui->dtCreation->date());
    m_audio->set_notes(ui->edtNotes->toPlainText().toStdString());

    fs::path p(ui->edtFilename->text().toStdString());

    if (!m_audio->audio_lib_path()->value().empty())
        m_audio->set_file_path(m_audio->audio_lib_path()->value());

    m_audio->audio_file().set_audio_filename(m_audio->audio_filename()->value());

    m_audio->audio_file().set_creation_date(m_audio->creation_date()->value().toString("dd/MM/yyyy").toStdString());
    m_audio->audio_file().set_audio_class(m_audio->audio_type()->displayName());
    m_audio->audio_file().set_genre(m_audio->genre()->displayName());
    m_audio->audio_file().set_year(m_audio->audio_year()->value());
    m_audio->audio_file().set_audio_title(m_audio->title()->value());
    m_audio->audio_file().set_artist_name(m_audio->artist_fullname());


}


void AudioForm::populateFormWidgets()
{

    ui->edtTitle->setText(stoq(m_audio->title()->value()));
    ui->edtArtist->setText(stoq(m_audio->artist()->displayName()));

    populate_choice_combo(ui->cbClass, m_audio->audio_type());

    ui->cbGenre->setModel(m_audio->genre()->dataModel());

    ui->cbGenre->setCurrentIndex( item_index(ui->cbGenre,
                                             stoq(m_audio->genre()->displayName())));

    ui->edtYear->setValue(m_audio->audio_year()->value());
    ui->edtFolder->setText(stoq(m_audio->folder()->displayName()));

    // Id -1 means this is a new file you are creating
    qDebug() << "Filepath: " << m_audio->file_path()->to_qstring();
    qDebug() << "Audio Lib Path: " << m_audio->audio_lib_path()->to_qstring();

    if (m_audio->id() != -1 )
        ui->edtFilename->setText(stoq(m_audio->file_path()->value() +
                       m_audio_tool->make_audio_filename(m_audio->id())+
                                      "."+m_audio->file_extension()->value()));

    ui->edtDuration->setText(m_audio_tool->format_time(m_audio->duration()->value()));


    ui->edtStart->setText(m_audio_tool->format_time(m_audio->start_marker()->value()));
    ui->edtFadeIn->setText(m_audio_tool->format_time(m_audio->fade_in_marker()->value()));
    ui->edtIntro->setText(m_audio_tool->format_time(m_audio->intro_marker()->value()));

    ui->edtExtro->setText(m_audio_tool->format_time(m_audio->extro_marker()->value()));
    ui->edtFadeOut->setText(m_audio_tool->format_time(m_audio->fade_out_marker()->value()));
    ui->edtEnd->setText(m_audio_tool->format_time(m_audio->end_marker()->value()));

    ui->edtNotes->setText(stoq(m_audio->notes()->value()));

    ui->dtCreation->setDate(m_audio->creation_date()->value());
}

int AudioForm::item_index(QComboBox* cbox, const QString item)
{
    int index = ui->cbGenre->findText(item);
    return ( index < 0) ? 0 : index;
}

void AudioForm::populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for(const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));
    set_choice_field_default(cbox, cf->value());
}

void AudioForm::set_choice_field_default(QComboBox* cbox, const std::string value)
{
    if (value.empty()){
        cbox->setCurrentIndex(0);
    }else{
        cbox->setCurrentIndex(cbox->findData(QVariant(stoq(value))));
    }
}

void AudioForm::pick_artist()
{
    PickListSetting set(std::make_unique<AUDIO::Artist>(), ArtistColumn::FullName);
    m_artist_picker = std::make_unique<PickListBrowser>(set);
    m_artist_picker->exec();
    for (const auto& entity : set.selectedEntities){
        auto artist = dynamic_cast<AUDIO::Artist*>(entity);

        auto artist_uptr = std::make_unique<AUDIO::Artist>();
        *artist_uptr.get() = *artist;

        m_audio->artist()->set_fk_entity(std::move(artist_uptr));

        m_audio->set_artist(artist->id());
        ui->edtArtist->setText(QString::fromStdString(artist->fullName()->value()));
    }
}

void AudioForm::genre_combo_changed(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbGenre->model());
    m_audio->genre()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}


