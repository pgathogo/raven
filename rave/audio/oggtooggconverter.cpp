#include "oggtooggconverter.h"
#include "audio.h"

namespace AUDIO
{
    OggToOggConverter::OggToOggConverter(std::shared_ptr<Audio> audio)
        :AudioConverter(QString::fromStdString(audio->audio_filename()->valueToString()))
         ,m_src_ogg_filename{QString::fromStdString(audio->audio_filename()->valueToString())}
    {

    }

    OggToOggConverter::~OggToOggConverter()
    {
    }


    bool OggToOggConverter::convert()
    {
    }

    QString OggToOggConverter::src_ogg_filename()
    {
        return m_src_ogg_filename;
    }

    void OggToOggConverter::set_ogg_filename(QString filename)
    {
        m_src_ogg_filename = filename;
    }



}
