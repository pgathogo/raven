#ifndef AUDIOCACHEMANAGER_H
#define AUDIOCACHEMANAGER_H

#include <memory>
#include <QFile>

#include "audiocache.h"
#include "audiotool.h"
#include "../utils/tools.h"

namespace AUDIO
{
    template<typename T>
    class AudioCacheManager
    {
    public:
        AudioCacheManager(const QString cach_db, const QString cache_audio_store);
        int cache_size();
        int load_cached_audio();
        T* db_manager();
        void populate_cache();
        void queue_audio(std::unique_ptr<AudioCache> audio_cache);
        void cache_audio();
        void print_queue();
        void print_cache();
        void queue_to_cache();
        void clear_queue();
        void clear_cache();
    private:
        std::unique_ptr<T> m_dbmanager;
        AudioCache m_audio_cache;
        std::vector<std::unique_ptr<AudioCache>> m_queue;
        std::vector<std::unique_ptr<AudioCache>> m_cache;
        QString m_cache_db;
        QString m_cache_audio_store;
    };

    template<typename T>
    AudioCacheManager<T>::AudioCacheManager(const QString cache_db, const QString cache_audio_store)
        :m_cache_db{cache_db}
        ,m_cache_audio_store{cache_audio_store}
    {
        m_dbmanager = std::make_unique<T>(cache_db.toStdString());
    }

    template<typename T>
    int AudioCacheManager<T>::cache_size()
    {
        return m_cache.size();
    }


    template<typename T>
    int AudioCacheManager<T>::load_cached_audio()
    {
       int data_count = m_dbmanager->fetchAll(m_audio_cache);

       if (data_count > 0){
           populate_cache();
       }

       return data_count;
    }

    template<typename T>
    void AudioCacheManager<T>::queue_audio(std::unique_ptr<AudioCache> audio_cache)
    {
        for (auto const& ac : m_queue){
            if (ac == audio_cache)
                return;
        }

        m_queue.push_back(std::move(audio_cache));

    }

    template<typename T>
    void AudioCacheManager<T>::cache_audio()
    {
        auto it = m_queue.begin();

        while (it != m_queue.end())
        {
            AudioTool at;

            qDebug() << "AUDIO ID: "<< (*it)->audio_id()->value();

            auto audio_name =  QString::fromStdString(at.make_audio_filename((*it)->audio_id()->value()));


            QString audio_filename = m_cache_audio_store+audio_name+".ogg";
            auto audio_adf = m_cache_audio_store+audio_name+".adf";
            auto audio_wave = m_cache_audio_store+audio_name+".png";

            // Handle copy failure
            if (!QFile::exists(audio_filename))
                    QFile::copy(stoq((*it)->orig_filepath()->value())+audio_filename,
                                 audio_filename);

            if (!QFile::exists(audio_adf))
                    QFile::copy(stoq((*it)->orig_filepath()->value())+audio_adf,
                                 audio_adf);

            if (!QFile::exists(audio_wave))
                    QFile::copy(stoq((*it)->orig_filepath()->value())+audio_wave,
                                 audio_wave);

            m_cache.push_back(std::move(*it));

            m_queue.erase(it);

        }

    }

   template<typename T>
   void AudioCacheManager<T>::print_queue()
   {
       printstr("---- QUEUE -----");
       for (auto const& ac : m_queue)
       {
           qDebug() << ac->audio_id()->value() << ": "<< stoq(ac->title()->value());
       }
       printstr("---- END-QUEUE -----");

   }

   template<typename T>
   void AudioCacheManager<T>::print_cache()
   {
       printstr("---- CACHED -----");
       for (auto const& ac : m_cache)
       {
           qDebug() << ac->audio_id()->value() << ": "<< stoq(ac->title()->value());
       }
       printstr("---- END-CACHED -----");
   }

  template<typename T>
  void AudioCacheManager<T>::queue_to_cache()
  {
    cache_audio();
  }

  template<typename T>
  void AudioCacheManager<T>::clear_queue()
  {
      m_queue.clear();
  }

  template<typename T>
  void AudioCacheManager<T>::clear_cache()
  {
      m_cache.clear();
  }

    template<typename T>
    void AudioCacheManager<T>::populate_cache()
    {
        auto data_provider = m_dbmanager->provider();

        data_provider->cache()->first();

        do{

           std::unique_ptr<AudioCache> ac = std::make_unique<AudioCache>();

           auto [column, value] = (*data_provider->cache()->currentElement()->begin());

           if (column == "id")
               ac->setId(str_to_int(value));

           if (column == "audio_id")
               ac->set_audio_id(str_to_int(value));

           if (column == "title")
               ac->set_title(value);

           if (column == "artist_name")
               ac->set_artist_name(value);

           if (column == "orig_filepath")
               ac->set_orig_filepath(value);

           if (column == "cache_filepath")
               ac->set_cache_filepath(value);

           if (column == "cache_datetime")
               ac->set_cache_datetime(QDateTime::fromString(QString::fromStdString(value)));

           if (column == "last_play_datatime")
               ac->set_last_play_datetime(QDateTime::fromString(QString::fromStdString(value)));

           if (column == "audio_type")
               ac->set_audio_type(value);

            m_cache.push_back(std::move(ac));

          data_provider->cache()->next();

        } while (!data_provider->cache()->isLast());


    }

    template<typename T>
    T* AudioCacheManager<T>::db_manager()
    {
        return m_dbmanager.get();
    }


}

#endif // AUDIOCACHEMANAGER_H
