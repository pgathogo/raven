#ifndef MTSTOMP3CONVERTER_H
#define MTSTOMP3CONVERTER_H

#include <memory>
#include "audioconverter.h"

namespace AUDIO
{
    class Audio;

    class MtsToMp3Converter : public AudioConverter
    {
    public:
        //MtsToMp3Converter(const QString);
        MtsToMp3Converter(const QString);
        ~MtsToMp3Converter() override;

        bool convert() override;
        QString ogg_filename() override;
        QString src_mts_filename();
        void set_mts_filename(const QString);
        QString dest_mp3_filename();
     private:
        QString m_src_mts_filename;
        QString m_dest_mp3_filename;
    };
}

#endif // MTSTOMP3CONVERTER_H
