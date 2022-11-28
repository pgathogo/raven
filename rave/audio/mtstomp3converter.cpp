#include <QFile>
#include <QFileInfo>
#include "mtstomp3converter.h"

namespace AUDIO
{
    MtsToMp3Converter::MtsToMp3Converter(const QString src_mts_file)
        :AudioConverter(src_mts_file)
         ,m_src_mts_filename{src_mts_file}
        ,m_dest_mp3_filename{}
    {
    }

    MtsToMp3Converter::~MtsToMp3Converter()
    {
    }

    bool MtsToMp3Converter::convert()
    {
        // rename file
        QFileInfo mts_file_info(m_src_mts_filename);
        QString basename = mts_file_info.baseName();
        QString path = mts_file_info.absolutePath();

        m_dest_mp3_filename = path+"/"+basename+".mp3";

        QFile::rename(m_src_mts_filename, m_dest_mp3_filename);


        return true;

    }

    QString MtsToMp3Converter::ogg_filename()
    {
       return m_dest_mp3_filename;
    }

    QString MtsToMp3Converter::src_mts_filename()
    {
        return m_src_mts_filename;
    }

    void MtsToMp3Converter::set_mts_filename(const QString filename)
    {
        m_src_mts_filename = filename;
    }

    QString MtsToMp3Converter::dest_mp3_filename()
    {
        return m_dest_mp3_filename;
    }
}
