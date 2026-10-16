#pragma once

#include <QWidget>

class QPushButton;
class QLabel;

namespace AUDIO {

    class AudioPropertyWidget;
    class AudioPlayer;
    class AudioMeter;

    class AudioToolBar : public QWidget
    {
        Q_OBJECT
    public:
        AudioToolBar();
    signals:
        void play_audio();
        void stop_audio();
        void cue_edit();
    private:
        void setupui();

        QPushButton* m_btn_play;
        QPushButton* m_btn_stop;
        QPushButton* m_btn_cue_edit;

    };

    class AudioProperty : public QWidget
    {
        Q_OBJECT
    public:
        AudioProperty(std::shared_ptr<Audio>);
        void update_properties(std::shared_ptr<Audio>);
    private:
        void setupui();
        std::shared_ptr<Audio> m_audio;
        QLabel* m_lblAudioRefValue;
        QLabel* m_lblTitleValue;
        QLabel* m_lblFilePathValue;
        QLabel* m_lblDurationValue;
    };

    class AudioPropertyWidget : public QWidget
    {
        Q_OBJECT
    public:
        AudioPropertyWidget(std::shared_ptr<Audio>);
        void update_widget(std::shared_ptr<Audio>);

    signals:
        void cue_edit_audio();

    private slots:
        void on_play_audio();
        void on_stop_audio();

    private:
        void setupui();

        AudioProperty* m_audio_prop;
        std::shared_ptr<Audio> m_audio;
        std::shared_ptr<AudioPlayer> m_audio_player;
        std::shared_ptr<AudioMeter> m_audio_meter;
    };

}
