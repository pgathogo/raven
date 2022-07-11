#pragma once

#include <memory>
#include "../audio/audio.h"
#include "../framework/choicefield.h"
#include "qstandarditemmodel.h"


class QStandardItemModel;
class QStandardItem;

using Columns = QList<QStandardItem*>;

namespace AUDIO{
    class Artist;
}

namespace AUDIO{

    class AudioLibItem{
    public:
        AudioLibItem();
        AudioLibItem(AUDIO::Audio*);
        AudioLibItem(QStandardItemModel*);
        ~AudioLibItem();

        virtual QString title();
        virtual QString artist();
        virtual QString audio_type();
        virtual QString duration();
        virtual QString genre();
        virtual QString audio_year();
        virtual QString file_path();
        virtual QString folder();

        AUDIO::Audio* audio() const;

        AUDIO::Audio* find_audio_by_id(int);

        std::unique_ptr<AUDIO::AudioLibItem> const& one_item() const;

        void clear();


        template<typename T>
        void create_row_item(AUDIO::Audio* audio)
        {
            static_assert(std::is_base_of<AUDIO::AudioLibItem, T>::value, "`T` must be derived from AudioLibItem");

            Columns columns;

            auto item = std::make_unique<T>(audio);

            auto title = new T(item->title());
            title->setData(audio->id(), Qt::UserRole);

            auto artist = new T(item->artist());
            auto duration = new T(item->duration());
            auto audio_type = new T(item->audio_type());
            auto file_path = new T(item->file_path());
            auto folder = new T(item->folder());

            if (audio->title()->visible())
                columns.append(title);
            if (audio->artist()->visible())
                columns.append(artist);
            if (audio->duration()->visible())
                columns.append(duration);
            if (audio->audio_type()->visible())
                columns.append(audio_type);
            if (audio->file_path()->visible())
                columns.append(file_path);
            if (audio->folder()->visible())
                columns.append(folder);

            m_model->appendRow(columns);
            m_lib_items.push_back(std::move(item));
        }
    private:
        AUDIO::Audio* m_audio;
        QStandardItemModel* m_model;
        std::vector<std::unique_ptr<AUDIO::AudioLibItem>> m_lib_items;
    };

    class SongAudioLibItem : public AudioLibItem, public QStandardItem
    {
     public:
        SongAudioLibItem(AUDIO::Audio*);
        SongAudioLibItem(QString);
        ~SongAudioLibItem();
    };

    class JingleAudioLibItem : public AudioLibItem, public QStandardItem
    {
    public:
        JingleAudioLibItem(AUDIO::Audio*);
        JingleAudioLibItem(QString);
        ~JingleAudioLibItem();

    };

    class CommercialAudioLibItem: public AudioLibItem, public QStandardItem
    {
    public:
        CommercialAudioLibItem(AUDIO::Audio*);
        CommercialAudioLibItem(QString);
        ~CommercialAudioLibItem();
    };

    struct FindAudioById{
        FindAudioById(int id)
            :m_id{ id }
        {
        }

        bool operator()(const std::unique_ptr<AUDIO::AudioLibItem>& audio_lib_item)
        {
            return (audio_lib_item->audio()->id() == m_id);
        }

     private:
        int m_id{-1};
    };

}
