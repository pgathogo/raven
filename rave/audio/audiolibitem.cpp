#include <QStandardItemModel>
#include <QStandardItem>

#include "audiolibitem.h"
#include "../audio/audiotool.h"
#include "../framework/choicefield.h"

namespace AUDIO{

    AudioLibItem::AudioLibItem()
        :m_audio{nullptr}
    {
    }

    AudioLibItem::AudioLibItem(AUDIO::Audio* audio)
        :m_audio{audio}
    {
    }

    AudioLibItem::AudioLibItem(QStandardItemModel* model)
        :m_model{model}
    {
    }

    AudioLibItem::~AudioLibItem()
    {
    }

    QString AudioLibItem::title()
    {
        return QString::fromStdString(audio()->title()->value());
    }


    QString AudioLibItem::artist()
    {
        return QString::fromStdString(audio()->artist()->displayName());
    }

    QString AudioLibItem::audio_type()
    {
        return QString::fromStdString(audio()->audio_type()->valueToString());
    }

    QString AudioLibItem::duration()
    {
        return QTime::fromMSecsSinceStartOfDay(
                    audio()->duration()->value()).toString("hh:mm:ss");
    }

    QString AudioLibItem::genre()
    {
        return "";
    }
    QString AudioLibItem::audio_year()
    {
        return QString::number(audio()->audio_year()->value());
    }

    QString AudioLibItem::file_path()
    {
        AUDIO::AudioTool audio_tool;
        return QString::fromStdString(audio()->audio_lib_path()->value() +
                                      audio_tool.make_audio_filename(audio()->id())+
                                      "."+audio()->file_extension()->value_tolower());

    }

    QString AudioLibItem::folder()
    {
        return QString::fromStdString(audio()->folder()->displayName());
    }

    QString AudioLibItem::file_extension()
    {
        return QString::fromStdString(audio()->file_extension()->displayName());
    }

    AUDIO::Audio* AudioLibItem::audio() const
    {
        return m_audio;
    }

    std::unique_ptr<AUDIO::AudioLibItem> const& AudioLibItem::one_item() const
    {
        return m_lib_items.at(0);
    }

    void AudioLibItem::clear()
    {
        m_lib_items.clear();
    }

    AUDIO::Audio* AudioLibItem::find_audio_by_id(int audio_id)
    {

        auto iter = std::find_if(m_lib_items.cbegin(), m_lib_items.cend(),
                                 FindAudioById(audio_id));

        if (iter != m_lib_items.end())
            return (*iter)->audio();

        return nullptr;

    }

    /*  ---- SongAudioLibItem ------ */

    SongAudioLibItem::SongAudioLibItem(AUDIO::Audio* audio)
        :AudioLibItem(audio)
    {
    }

    SongAudioLibItem::SongAudioLibItem(QString text)
        :AudioLibItem()
    {
        setText(text);
    }

    SongAudioLibItem::~SongAudioLibItem()
    {
    }

 /* ------- Jingle -------------- */

    JingleAudioLibItem::JingleAudioLibItem(AUDIO::Audio* audio)
        :AudioLibItem(audio)
    {
    }

    JingleAudioLibItem::JingleAudioLibItem(QString text)
        :AudioLibItem()
    {
        setText(text);
        setForeground(QBrush(QColor(6,144,2)));
        //setForeground(QBrush(QColor(255,255,255)));
    }

    JingleAudioLibItem::~JingleAudioLibItem()
    {
    }

/* ---------  CommercialAudioLibItem -------------- */

    CommercialAudioLibItem::CommercialAudioLibItem(AUDIO::Audio* audio)
        :AudioLibItem(audio)
    {
    }

    CommercialAudioLibItem::CommercialAudioLibItem(QString text)
        :AudioLibItem()
    {
        setText(text);
        setForeground(QBrush(QColor(144,2,9)));
        //setForeground(QBrush(QColor(255,255,255)));
    }

    CommercialAudioLibItem::~CommercialAudioLibItem()
    {
    }

}
