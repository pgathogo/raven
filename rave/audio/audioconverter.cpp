#include <QFileInfo>

#include "audioconverter.h"


namespace AUDIO{

    AudioConverter::AudioConverter(QString audio_file)
        :m_audio_filename{audio_file}
    {
        set_ogg_filename(audio_file);
    }

    QString AudioConverter::ogg_filename()
    {
        return m_ogg_filename;
    }

    void AudioConverter::set_ogg_filename(QString audio_file)
    {
        QFileInfo fi(audio_file);
        auto abs_path = fi.absolutePath();
        auto base_name = fi.baseName();
        m_ogg_filename = abs_path+"/"+base_name+".ogg";
    }

    void AudioConverter::remove_ogg_file()
    {
        if (QFileInfo::exists(m_ogg_filename)){
            QFile::remove(m_ogg_filename);
        }
    }

    QString AudioConverter::audio_filename()
    {
        return m_audio_filename;
    }
}
