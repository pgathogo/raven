#pragma once

#include <QDialog>

#include "../framework/baseentitydetaildlg.h"

class QLineEdit;
class QTextEdit;

namespace AUDIO
{
class Audio;
class AudioWaveWidget;

class SpotAudioForm : public QDialog
{
    Q_OBJECT

public:
    explicit SpotAudioForm(std::shared_ptr<Audio>,
                           const QString&,
                           const QString&,
                           QDialog* parent=nullptr);
protected slots:
    void accept() override;

private:
    void setupui();

    std::shared_ptr<Audio> m_audio;
    QLineEdit* m_edtTitle;
    QTextEdit* m_edtNotes;
    AudioWaveWidget* m_audio_wave_widget;

    // Title
    // Notes
    // Duration
    // Markers

};



}