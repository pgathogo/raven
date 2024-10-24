#include "oggtooggconverter.h"
#include "audio.h"

namespace AUDIO
{
    OggToOggConverter::OggToOggConverter(const QString audio_filename)
        :AudioConverter(audio_filename)
         ,m_src_ogg_filename{audio_filename}
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
