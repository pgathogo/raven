#include "oggtooggconverter.h"
#include "audio.h"

namespace AUDIO
{
   OggToOggConverter::OggToOggConverter()
    :AudioConverter("")
   {
   }

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


    bool OggToOggConverter::convert(const QString &sourcePath, const QString &destPath, bool overwrite)
    {
        // 1. Check if the source file exists
        if (!QFile::exists(sourcePath)) {
            qWarning() << "Source file does not exist:" << sourcePath;
            return false;
        }

        // 2. Handle existing destination file if overwrite is requested
        if (QFile::exists(destPath)) {
            if (overwrite) {
                if (!QFile::remove(destPath)) {
                    qWarning() << "Failed to remove existing destination file:" << destPath;
                    return false;
                }
            } else {
                qWarning() << "Destination file already exists:" << destPath;
                return false;
            }
        }

        // 3. Perform the copy operation
        if (QFile::copy(sourcePath, destPath)) {
            qDebug() << "File successfully copied to:" << destPath;
            return true;
        } else {
            qWarning() << "Failed to copy file from" << sourcePath << "to" << destPath;
            return false;
        }
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
