#pragma once

#include <memory>

#include "audioconverter.h"

namespace AUDIO
{
   class Audio;

    class OggToOggConverter : public AudioConverter
    {
    public:
        //OggToOggConverter(const QString src_ogg_file);
        OggToOggConverter();
        OggToOggConverter(const QString);
        ~OggToOggConverter() override;

        bool convert() override;
        bool convert(const QString&, const QString&, bool overwrite=true);
        QString src_ogg_filename();
        void set_ogg_filename(QString);
    private:
        QString m_src_ogg_filename;
    };
}

