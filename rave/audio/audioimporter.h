#pragma once

#include <QString>
#include <QApplication>

#include "audiofile.h"

namespace AUDIO{
   class Audio;
}

class RavenSetup;

namespace AUDIO{

    class AudioImporter
    {
    public:
        AudioImporter(AUDIO::Audio* audio, QApplication* qapp);
        void import_audio();
        void create_audio_wave_file();
        void write_ogg_file(int);
        void write_wave_file(int);
        void create_adf_file(int);

        void remove_ogg_temp_file();
        void remove_wave_temp_file();

        AudioFile audio_file();
        CueMarker marker();

    private:
        AUDIO::Audio* m_audio;
        QApplication* m_qapp;
        std::unique_ptr<EntityDataModel> m_setup_edm;
        RavenSetup* m_setup;

        AudioFile m_audio_file;
        CueMarker m_marker;

        void open_cue_editor();
    };
}

