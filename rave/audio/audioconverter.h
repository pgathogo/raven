#pragma once

#include <QObject>

namespace AUDIO{

    class AudioConverter : public QObject
    {
    public:
        AudioConverter(QString audio_file);
        virtual void convert() = 0;
        virtual QString audio_filename();
        virtual QString ogg_filename();
        virtual void remove_ogg_file();
        void set_ogg_filename(QString);
    private:
        QString m_audio_filename;
        QString m_ogg_filename;
    };
}

