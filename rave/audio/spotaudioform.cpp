#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpacerItem>
#include <QDialogButtonBox>
#include <QDate>

#include "../framework/entitydatamodel.h"
#include "../framework/ravensetup.h"

#include "spotaudioform.h"
#include "audio.h"
#include "audiowavewidget.h"

namespace AUDIO {

SpotAudioForm::SpotAudioForm(std::shared_ptr<Audio> audio,
                             const QString& s_title,
                             const QString& c_name,
                             QDialog* parent)
    :QDialog(parent)
    ,m_audio{audio}
{
    setupui();

    m_edtTitle->setText(QString::fromStdString(m_audio->title()->value()));

    QString spot_title = QString("Spot Title: %1").arg(s_title);
    QString client_name = QString("Client Name: %1").arg(c_name);

    m_edtNotes->append(spot_title);
    m_edtNotes->append(client_name);

}

void SpotAudioForm::setupui()
{
    qDebug() << " BBB ";

    setWindowTitle("Spot Audio");
    setMinimumSize(1380, 700);

    QVBoxLayout* main_layout = new QVBoxLayout();

    QGridLayout* prop_layout = new QGridLayout();

    QLabel* lblTitle = new QLabel("Title:");
    m_edtTitle = new QLineEdit();

    /* Spot Title: "Spot Title"
     * Client    : "Client Name"
    */

    QLabel* lblNotes = new QLabel("Notes:");
    m_edtNotes = new QTextEdit();
    m_edtNotes->setMaximumHeight(100);

    prop_layout->addWidget(lblTitle, 0, 0);
    prop_layout->addWidget(m_edtTitle, 0, 1);
    prop_layout->addWidget(lblNotes, 1, 0);
    prop_layout->addWidget(m_edtNotes, 1, 1);

    QSpacerItem* hSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    prop_layout->addItem(hSpacer, 0, 2, 2, 1);

    prop_layout->setColumnStretch(0, 0);
    prop_layout->setColumnStretch(1, 1);
    prop_layout->setColumnStretch(2, 2);

    main_layout->addLayout(prop_layout);

    QString audio_filepath = "";
    if (m_audio->id() == -1) {
        audio_filepath =  QString::fromStdString(m_audio->audio_filename()->value());
    } else {
        audio_filepath =  m_audio->full_audio_filename();
    }
    qDebug() << "Audio File: " << audio_filepath;
    m_audio_wave_widget = new AUDIO::AudioWaveWidget(audio_filepath);
    main_layout->addWidget(m_audio_wave_widget);

    QHBoxLayout* hl_footer = new QHBoxLayout;
    QDialogButtonBox* btn_box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(btn_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

    hl_footer->addStretch();
    hl_footer->addWidget(btn_box);

    main_layout->addLayout(hl_footer);

    main_layout->addStretch();

    setLayout(main_layout);

}

void SpotAudioForm::accept()
{
    if (m_edtTitle->text().isEmpty()) {
        QMessageBox::warning(this, "Spot Audio", "Please enter audio title");
        return;
    }

    auto setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
    setup_edm->all();

    auto setup = std::dynamic_pointer_cast<RavenSetup>(setup_edm->firstEntity());

    m_audio->set_title(m_edtTitle->text().toStdString());
    m_audio->set_notes(m_edtNotes->toPlainText().toStdString());
    m_audio->set_duration(m_audio_wave_widget->audio_duration() * 1000.0); // msecs
    m_audio->set_audio_type("COMM");
    m_audio->set_file_path(setup->comm_audio_folder()->value());
    m_audio->set_audio_year(QDate::currentDate().year());
    m_audio->set_artist(-1);
    m_audio->set_folder(1); // TODO: Get folder from the RavenSetup ->
    m_audio->set_creation_date(QDate::currentDate());
    m_audio->set_file_extension("OGG");

    QDialog::accept();

}

}