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
        OggToOggConverter(std::shared_ptr<Audio>);
        ~OggToOggConverter() override;

        bool convert() override;
        QString src_ogg_filename();
        void set_ogg_filename(QString);
    private:
        QString m_src_ogg_filename;
    };
}

