#include "oggtooggconverter.h"

namespace AUDIO
{
    OggToOggConverter::OggToOggConverter(const QString src_ogg_file)
        :AudioConverter(src_ogg_file)
         ,m_src_ogg_filename{src_ogg_file}
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
