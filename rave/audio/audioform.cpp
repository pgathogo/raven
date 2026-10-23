#include "audioform.h"

#include <memory>

#include "../audio/audiowavewidget.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiotool.h"
#include "../utils/tools.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"
#include "../framework/picklistbrowser.h"
#include "../framework/ravensetup.h"

#include "../../AudioExplorer/artistform.h"


#include <QDialogButtonBox>
#include <QFont>
#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QJsonObject>

namespace AUDIO
{

    AudioForm::AudioForm(std::shared_ptr<AUDIO::Audio> audio,
                         QWidget *parent)
        : QDialog(parent)
     ,m_audio{audio}
    {
        setupUi();
        populate_widgets();
    }

    void AudioForm::populate_widgets()
    {
        // Title
        m_titleEdit->setText(m_audio->title()->to_qstring());
        m_artistEdit->setText(stoq(m_audio->artist()->displayName()));
        populate_choice_combo(m_classCombo, m_audio->audio_type());

        int genre_index = m_audio->genre()->value();

        m_genreCombo->setModel(m_audio->genre()->dataModel());

        if (genre_index > 0)
            m_genreCombo->setCurrentIndex(genre_index-1);

        m_yearSpin->setValue(m_audio->audio_year()->value());
        m_notesEdit->setPlainText(m_audio->notes()->to_qstring());


        QString audio_id="0";
        AudioTool at;
        if (m_audio->id() > -1)
            audio_id = QString::fromStdString(at.make_audio_filename(m_audio->id())+"."+m_audio->file_extension()->value() );
        m_fileNameValue->setText(audio_id);
        m_folderValue->setText(m_audio->file_path()->to_qstring());

        QString DFMT = "dd-MM-yyyy";
        QString current_date = QDate::currentDate().toString(DFMT);
        if (m_audio->creation_date()->value().isValid())
            current_date = m_audio->creation_date()->value().toString(DFMT);
        m_creationDateValue->setText(current_date);

        // Load markers


    }

    QLabel *AudioForm::createValueLabel(const QString &text)
    {
        auto *label = new QLabel(text);
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        return label;
    }

    void AudioForm::setupUi()
    {
        setWindowTitle(QString("Audio Form - %1").arg(m_audio->title()->to_qstring()));

        resize(800, 380);

        // ---- Create widgets -------------------------------------------------
        m_titleEdit = new QLineEdit;

        m_artistEdit = new QLineEdit;
        m_selectArtistButton = new QPushButton(tr("Select Artist..."));

        m_classCombo = new QComboBox;
        m_genreCombo = new QComboBox;

        connect(m_genreCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, &AudioForm::genre_combo_changed);


        m_yearSpin = new QSpinBox;
        m_yearSpin->setRange(0, 9999);
        m_yearSpin->setValue(0);

        m_notesEdit = new QPlainTextEdit;

        m_fileNameValue = createValueLabel("0000001");
        m_folderValue = createValueLabel("10.10.22.5\\Volume1\\Audio\\Folder\\Audio02");
        m_creationDateValue = createValueLabel("01-10-2026");

        m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        // ---- Labels with buddies (gives Alt+key shortcuts) -------------------
        auto *titleLabel = new QLabel(tr("&Title:"));
        titleLabel->setBuddy(m_titleEdit);

        auto *artistLabel = new QLabel(tr("&Artist:"));
        artistLabel->setBuddy(m_artistEdit);

        auto *classLabel = new QLabel(tr("&Class:"));
        classLabel->setBuddy(m_classCombo);

        auto *genreLabel = new QLabel(tr("&Genre:"));
        genreLabel->setBuddy(m_genreCombo);

        auto *yearLabel = new QLabel(tr("&Year:"));
        yearLabel->setBuddy(m_yearSpin);

        auto *notesLabel = new QLabel(tr("&Notes:"));
        notesLabel->setBuddy(m_notesEdit);

        // ---- Class / Genre / Year row ----------------------------------------
        auto *classGenreYearLayout = new QHBoxLayout;
        classGenreYearLayout->addWidget(m_classCombo, 1);
        classGenreYearLayout->addSpacing(8);
        classGenreYearLayout->addWidget(genreLabel);
        classGenreYearLayout->addWidget(m_genreCombo, 1);
        classGenreYearLayout->addSpacing(8);
        classGenreYearLayout->addWidget(yearLabel);
        classGenreYearLayout->addWidget(m_yearSpin);

        // ---- Left side: editable fields --------------------------------------
        auto *leftLayout = new QGridLayout;
        leftLayout->setHorizontalSpacing(8);
        leftLayout->setVerticalSpacing(14);

        // Title (spans edit + button columns)
        leftLayout->addWidget(titleLabel, 0, 0);
        leftLayout->addWidget(m_titleEdit, 0, 1, 1, 2);

        // Artist + Select button
        leftLayout->addWidget(artistLabel, 1, 0);
        leftLayout->addWidget(m_artistEdit, 1, 1);
        leftLayout->addWidget(m_selectArtistButton, 1, 2);

        // Class / Genre / Year
        leftLayout->addWidget(classLabel, 2, 0);
        leftLayout->addLayout(classGenreYearLayout, 2, 1, 1, 2);

        // Notes (label aligned to the top of the multi-line edit)
        leftLayout->addWidget(notesLabel, 3, 0, Qt::AlignTop);
        leftLayout->addWidget(m_notesEdit, 3, 1, 1, 2);

        leftLayout->setColumnStretch(1, 1);
        leftLayout->setRowStretch(3, 1);

        // ---- Right side: read-only info --------------------------------------
        auto *rightLayout = new QFormLayout;
        rightLayout->setHorizontalSpacing(16);
        rightLayout->setVerticalSpacing(14);
        rightLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        rightLayout->addRow(tr("File Name:"), m_fileNameValue);
        rightLayout->addRow(tr("Folder:"), m_folderValue);
        rightLayout->addRow(tr("Creation Date:"), m_creationDateValue);

        // Keep the info block pinned to the top of the right column
        auto *rightColumn = new QVBoxLayout;
        rightColumn->addLayout(rightLayout);
        rightColumn->addStretch(1);

        // ---- Content area: left (stretches) + right (fixed) ------------------
        auto *contentLayout = new QHBoxLayout;
        contentLayout->setSpacing(16);
        contentLayout->addLayout(leftLayout, 1);
        contentLayout->addLayout(rightColumn, 0);

        // --- AudioWaveWidget ---
        QString audio_filepath = get_audio_file();
        m_audio_wave_widget = new AUDIO::AudioWaveWidget(audio_filepath);
        connect(m_audio_wave_widget, &AudioWaveWidget::wave_is_loaded, this, &AudioForm::show_markers);

        // ---- Main layout -----------------------------------------------------
        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(contentLayout, 1);

        QFrame* hline = new QFrame(this);
        hline->setFrameShape(QFrame::HLine);
        hline->setFrameShadow(QFrame::Sunken);

        mainLayout->addWidget(hline);
        mainLayout->addWidget(m_audio_wave_widget);
        mainLayout->addWidget(m_buttonBox);

        // ---- Connections -----------------------------------------------------
        connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
        connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
        connect(m_selectArtistButton, &QPushButton::clicked, this, &AudioForm::pick_artist);

        m_titleEdit->setFocus();
    }

    void AudioForm::show_markers()
    {
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::Start, m_audio->start_marker()->value());
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::FadeIn, m_audio->fade_in_marker()->value());
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::Intro, m_audio->intro_marker()->value());
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::Extro, m_audio->extro_marker()->value());
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::FadeOut, m_audio->fade_out_marker()->value());
        m_audio_wave_widget->set_marker(AudioWaveWidget::MarkerType::End, m_audio->end_marker()->value());
    }

    void AudioForm::accept()
    {
        auto setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
        setup_edm->all();
        auto setup = std::dynamic_pointer_cast<RavenSetup>(setup_edm->firstEntity());

        set_choice_field(m_audio->audio_type(), m_classCombo);

        m_audio->set_title(m_titleEdit->text().toStdString());
        m_audio->set_file_path(setup->audio_folder()->value());
        m_audio->set_audio_type("SONG");
        m_audio->set_file_extension("OGG");
        m_audio->set_audio_year(m_yearSpin->value());
        m_audio->set_notes(m_notesEdit->toPlainText().toStdString());
        m_audio->set_duration(m_audio_wave_widget->audio_duration_ms());
        m_audio->set_creation_date(QDate::currentDate());
        m_audio->set_add_login("AudioExplorer"); // TODO: Get the actual user name

        // Markers
        auto markers = m_audio_wave_widget->markers();

        auto mark_to_ms = [](double sec) -> double {
              return sec * 1000;
        };

        m_audio->set_start_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::Start]));
        m_audio->set_fade_in_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::FadeIn]));
        m_audio->set_intro_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::Intro]));
        m_audio->set_extro_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::Extro]));
        m_audio->set_fade_out_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::FadeOut]));
        m_audio->set_end_marker(mark_to_ms(markers[AUDIO::AudioWaveWidget::MarkerType::End]));

        QDialog::accept();

    }

    QString AudioForm::get_audio_file()
    {
        return (m_audio->id() > -1) ? m_audio->full_audio_filename() : m_audio->audio_filename()->to_qstring();
    }

    // ---- Accessors -----------------------------------------------------------

    QString AudioForm::title() const { return m_titleEdit->text(); }
    void AudioForm::setTitle(const QString &title) { m_titleEdit->setText(title); }

    QString AudioForm::artist() const { return m_artistEdit->text(); }
    void AudioForm::setArtist(const QString &artist) { m_artistEdit->setText(artist); }

    QString AudioForm::trackClass() const { return m_classCombo->currentText(); }
    void AudioForm::setTrackClass(const QString &trackClass)
    {
        m_classCombo->setCurrentText(trackClass);
    }
    void AudioForm::setClasses(const QStringList &classes)
    {
        m_classCombo->clear();
        m_classCombo->addItems(classes);
    }

    QString AudioForm::genre() const { return m_genreCombo->currentText(); }
    void AudioForm::setGenre(const QString &genre) { m_genreCombo->setCurrentText(genre); }

    void AudioForm::setGenres(const QStringList &genres)
    {
        m_genreCombo->clear();
        m_genreCombo->addItems(genres);
    }

    int AudioForm::year() const { return m_yearSpin->value(); }
    void AudioForm::setYear(int year) { m_yearSpin->setValue(year); }

    QString AudioForm::notes() const { return m_notesEdit->toPlainText(); }
    void AudioForm::setNotes(const QString &notes) { m_notesEdit->setPlainText(notes); }

    void AudioForm::setFileName(const QString &fileName) { m_fileNameValue->setText(fileName); }
    void AudioForm::setFolder(const QString &folder) { m_folderValue->setText(folder); }
    void AudioForm::setCreationDate(const QDate &date)
    {
        m_creationDateValue->setText(date.toString(QStringLiteral("dd-MM-yyyy")));
    }

    void AudioForm::populate_choice_combo(QComboBox* cbox, const ChoiceField<std::string>* cf)
    {
        for(const auto& c : cf->choices())
            cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));
        set_choice_field_default(cbox, cf->value());
    }

    int AudioForm::item_index(QComboBox* cbox, const QString item)
    {
        int index =cbox->findText(item);
        return ( index < 0) ? 0 : index;
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
        set.can_add_new = true;

        m_artist_picker = std::make_unique<PickListBrowser>(set);
        m_artist_picker->exec();
        for (const auto& entity : set.selectedEntities){
            auto artist = dynamic_cast<AUDIO::Artist*>(entity);
            auto artist_uptr = std::make_unique<AUDIO::Artist>();
            *artist_uptr.get() = *artist;

            // auto artist = dynamic_pointer_cast<AUDIO::Artist>(entity);
            // auto artist_uptr = std::make_unique<AUDIO::Artist>();
            // *artist_uptr(artist);

            m_audio->artist()->set_fk_entity(std::move(artist_uptr));

            m_audio->set_artist(artist->id());
            m_artistEdit->setText(artist->fullName()->to_qstring());
        }
    }

    void AudioForm::genre_combo_changed(int i)
    {
        EntityDataModel* edm = dynamic_cast<EntityDataModel*>(m_genreCombo->model());
        m_audio->genre()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
    }



}