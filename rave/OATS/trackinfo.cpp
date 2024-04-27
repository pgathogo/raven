#include <chrono>
#include <QTime>

#include "../audio/audio.h"
#include "../audio/audiotool.h"
#include "../audio/audiohistoryform.h"
#include "../framework/choicefield.h"

#include "../utils/tools.h"

#include "trackinfo.h"
#include "ui_trackinfo.h"

TrackInfo::TrackInfo(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::TrackInfo)
    ,m_audio_history{nullptr}
{
    ui->setupUi(this);

    m_audio_history = std::make_unique<AudioHistoryForm>();
    ui->vlHistory->addWidget(m_audio_history.get());
    style_text();
}

TrackInfo::~TrackInfo()
{
    delete ui;
}

void TrackInfo::show_audio_info(const std::shared_ptr<AUDIO::Audio> audio, History& history)
{

    ui->lblTitle->setText(stoq(audio->title()->value()));
    ui->lblArtist->setText(stoq(audio->artist()->displayName()));

    QTime t = msec_to_time(0, audio->duration()->value());
    ui->lblDuration->setText(t.toString("HH:mm:ss"));

    ui->lblClass->setText(stoq(audio->audio_type()->displayName()));
    ui->lblYear->setText(QString::number(audio->audio_year()->value()));

    AUDIO::AudioTool at;
    auto ogg_file =  at.make_audio_filename(audio->id())+".ogg";
    ui->lblAudioFile->setText( stoq(audio->audio_lib_path()->value()+ogg_file) );

    ui->lblFadeIn->setText(msec_to_time(0, audio->fade_in_marker()->value()).toString("HH:mm:ss"));
    ui->lblIntro->setText(msec_to_time(0, audio->intro_marker()->value()).toString("HH:mm:ss"));
    ui->lblExtro->setText(msec_to_time(0, audio->extro_marker()->value()).toString("HH:mm:ss"));
    ui->lblFadeOut->setText(msec_to_time(0, audio->fade_out_marker()->value()).toString("HH:mm:ss"));
    ui->lblEnd->setText(msec_to_time(0, audio->end_marker()->value()).toString("HH:mm:ss"));

    m_audio_history->show_history(&history);
    m_audio_history->show();


}

void TrackInfo::style_text()
{
    QString text_label_style(
        "QLabel{color: #0479B0;}"
        );

    QString value_label_style(
        "QLabel{color: #FFFFFF;}"
        );

    QString text_time_style(
        "QLabel{color: #0479B0;}"
        );

    QString value_time_style(
        "QLabel{color: #FFFFFF;}"
        );

    ui->txtTitle->setStyleSheet(text_label_style);
    ui->txtArtist->setStyleSheet(text_label_style);
    ui->txtClass->setStyleSheet(text_label_style);
    ui->txtDuration->setStyleSheet(text_label_style);
    ui->txtYear->setStyleSheet(text_label_style);
    ui->txtAudioFile->setStyleSheet(text_label_style);

    ui->lblTitle->setStyleSheet(value_label_style);
    ui->lblArtist->setStyleSheet(value_label_style);
    ui->lblDuration->setStyleSheet(value_label_style);
    ui->lblClass->setStyleSheet(value_label_style);
    ui->lblYear->setStyleSheet(value_label_style);
    ui->lblAudioFile->setStyleSheet(value_label_style);

    ui->txtFadeIn->setStyleSheet(text_time_style);
    ui->txtIntro->setStyleSheet(text_time_style);
    ui->txtExtro->setStyleSheet(text_time_style);
    ui->txtFadeOut->setStyleSheet(text_time_style);
    ui->txtEnd->setStyleSheet(text_time_style);

    ui->lblFadeIn->setStyleSheet(value_time_style);
    ui->lblIntro->setStyleSheet(value_time_style);
    ui->lblExtro->setStyleSheet(value_time_style);
    ui->lblFadeOut->setStyleSheet(value_time_style);
    ui->lblEnd->setStyleSheet(value_time_style);

    ui->tabWidget->setStyleSheet("QTabBar::tab{background: qlineargradient(x1:0 y1:0, x2:0 y2:1,"
                 " stop:0 #47617B, stop:0.5 #2C3C4B, stop:1 #10161C);"
                "color:#FFFFFF; font-weight: bold;}"
                "QTabBar::tab:selected{"
                   "background-color: qlineargradient(x1:0 y1:0, x2:0 y2:1, "
                "stop:0 #454500 , stop:1 #777700 );"
                "color:#FFFFFF; font-weight: bold;}"
                "QTabWidget::pane{background-color: #34424F; border: 1px solid lightgray;}"
                "QTabBar::tab:hover{background-color:#555D64;} "
                 );
}

QTime TrackInfo::msec_to_time(int h, int ms)
{
    auto msecs = std::chrono::milliseconds(ms);
    auto hr = std::chrono::duration_cast<std::chrono::hours>(msecs);
    msecs -= hr;
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(msecs);
    msecs -= mins;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(msecs);
    QTime t(h, mins.count(), secs.count());
    return t;
}
