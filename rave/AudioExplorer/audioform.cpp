#include <filesystem>
#include <QFileInfo>
#include "audioform.h"
#include "ui_audioform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/choicefield.h"
#include "../framework/picklistbrowser.h"

#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotool.h"

namespace fs = std::filesystem;

AudioForm::AudioForm(AUDIO::Audio* audio,
                     QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::AudioForm)
    ,m_audio{audio}
    ,m_artist_picker{nullptr}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    setMaximumWidth(500);
    setMaximumHeight(455);

    populateFormWidgets();

    connect(ui->btnPickArtist, &QPushButton::clicked, this, &AudioForm::pick_artist);
    connect(ui->cbGenre, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AudioForm::genre_combo_changed);
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
    return "Audio Details";
}

void AudioForm::populateEntityFields()
{
    m_audio->set_title(ui->edtTitle->text().toStdString());
    set_choice_field(m_audio->audio_type(), ui->cbClass);
    m_audio->set_audio_year(ui->edtYear->value());
    m_audio->set_add_dtime(ui->dtCreation->dateTime());
    m_audio->set_creation_date(ui->dtCreation->date());

    fs::path p(ui->edtFilename->text().toStdString());
    m_audio->set_file_path(p.parent_path().u8string()+"/");
}

void AudioForm::populateFormWidgets()
{

    ui->edtTitle->setText(stoq(m_audio->title()->value()));
    ui->edtArtist->setText(stoq(m_audio->artist()->displayName()));
    populate_choice_combo(ui->cbClass, m_audio->audio_type());
    ui->cbGenre->setModel(m_audio->genre()->dataModel());
    ui->cbGenre->setCurrentIndex(ui->cbGenre->findText(
                                     stoq(m_audio->genre()->displayName())));

    ui->edtYear->setValue(m_audio->audio_year()->value());
    ui->edtFolder->setText(stoq(m_audio->folder()->displayName()));

    AudioTool at;
    ui->edtFilename->setText(stoq(m_audio->audio_lib_path()->value() +
                                   at.make_audio_filename(m_audio->id())+".ogg"));
    ui->edtDuration->setText(at.format_time(m_audio->duration()->value()));

    ui->edtStart->setText(at.format_time(m_audio->start_marker()->value()));
    ui->edtFadeIn->setText(at.format_time(m_audio->fade_in_marker()->value()));
    ui->edtIntro->setText(at.format_time(m_audio->intro_marker()->value()));

    ui->edtExtro->setText(at.format_time(m_audio->extro_marker()->value()));
    ui->edtFadeOut->setText(at.format_time(m_audio->fade_out_marker()->value()));
    ui->edtEnd->setText(at.format_time(m_audio->end_marker()->value()));

    ui->edtNotes->setText(stoq(m_audio->notes()->value()));

    ui->dtCreation->setDate(m_audio->creation_date()->value());
}

void AudioForm::populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for(const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));
    set_choice_field_default(cbox, cf->value());
}

void AudioForm::set_choice_field_default(QComboBox* cbox, const std::string value)
{
    cbox->setCurrentIndex(cbox->findData(QVariant(stoq(value))));
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

