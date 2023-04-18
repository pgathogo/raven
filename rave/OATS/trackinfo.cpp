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
