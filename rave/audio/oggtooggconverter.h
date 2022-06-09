#pragma once

#include "audioconverter.h"

namespace AUDIO
{
    class OggToOggConverter : public AudioConverter
    {
    public:
        OggToOggConverter(const QString src_ogg_file);
        ~OggToOggConverter() override;

        void convert() override;
        QString src_ogg_filename();
        void set_ogg_filename(QString);
    private:
        QString m_src_ogg_filename;
    };
}

