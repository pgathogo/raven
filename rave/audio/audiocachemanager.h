#ifndef AUDIOCACHEMANAGER_H
#define AUDIOCACHEMANAGER_H

#include <memory>
#include <algorithm>
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
        void queue_cache(std::shared_ptr<AudioCache> audio);
        void cache_audio();
        void print_queue();
        void print_cache();
        void queue_to_cache();
        void clear_queue();
        void clear_cache();
        void persist_cache();
        std::string get_cached_audio_path(int);
        void set_cache_audio_last_play_dtime(int);
    private:
        std::unique_ptr<T> m_dbmanager;
        Audio m_cached_audio;
        std::vector<std::shared_ptr<AudioCache>> m_queue;
        std::vector<std::shared_ptr<AudioCache>> m_cache;
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
       int data_count = m_dbmanager->fetchAll(m_cached_audio);

       qDebug() << "load_cached_audio: "<< data_count;

       if (data_count > 0){
           populate_cache();
       }

       return data_count;
    }


    template<typename T>
    void AudioCacheManager<T>::queue_cache(std::shared_ptr<AudioCache> cache)
    {

        for (auto const& ac : m_queue){
            if (ac == cache)
                return;
        }

        m_queue.push_back(std::move(cache));

    }

    template<typename T>
    void AudioCacheManager<T>::cache_audio()
    {
        auto it = m_queue.begin();

        while (it != m_queue.end())
        {
            if (std::find_if(m_cache.begin(),  m_cache.end(),
                          [&](auto const&  ac){return ((*it)->audio_id()->value() == ac->audio_id()->value());
                             }) != m_cache.end())
            {
                m_queue.erase(it);
                continue;
            }

            AudioTool at;

            auto audio_name =  QString::fromStdString(at.make_audio_filename((*it)->audio_id()->value()));

            auto ogg_name = audio_name+"."+QString::fromStdString((*it)->audio()->file_extension()->value());
            auto adf_name = audio_name+".adf";
            auto png_name = audio_name+".png";

            QString file_path = stoq((*it)->audio()->audio_lib_path()->value());

            QString orig_audio_filename = file_path+ogg_name;
            auto orig_audio_adf = file_path+adf_name;
            auto orig_audio_wave = file_path+png_name;

            QString cache_audio_filename = m_cache_audio_store+ogg_name;
            auto cache_audio_adf = m_cache_audio_store+adf_name;
            auto cache_audio_wave = m_cache_audio_store+png_name;

            // TODO: Handle copy failure

            bool is_copied = false;

            if (!QFile::exists(cache_audio_filename)){
                    is_copied = QFile::copy(orig_audio_filename, cache_audio_filename);
                    // TODO: log this event
            }

            if (!QFile::exists(cache_audio_adf))
                    is_copied = QFile::copy(orig_audio_adf+adf_name, cache_audio_adf);

            if (!QFile::exists(cache_audio_wave))
                    is_copied = QFile::copy(orig_audio_wave+png_name, cache_audio_wave);

            (*it)->set_is_cached(true);
            (*it)->set_cache_filepath(m_cache_audio_store.toStdString());

            m_dbmanager->createEntity(*((*it).get()));

            m_cache.push_back(std::move(*it));

            m_queue.erase(it);

        }

    }

   template<typename T>
   void AudioCacheManager<T>::print_queue()
   {
       printstr("---- QUEUE -----");
       qDebug() << "Queue Size: "<< m_queue.size();

       for (auto const& cache : m_queue)
       {
           qDebug() << cache->audio_id()->value() << ": "<< stoq(cache->audio()->title()->value());
       }

       printstr("---- END-QUEUE -----");

   }

   template<typename T>
   void AudioCacheManager<T>::print_cache()
   {
       printstr("---- CACHED -----");

       for (auto const& cached_audio : m_cache)
       {
           qDebug() << "Audio ID: " << cached_audio->audio_id()->value();
           qDebug() << "*Title*: " << stoq(cached_audio->audio()->title()->value());
           qDebug() << "Artist Name: " << stoq(cached_audio->audio()->artist_fullname());

           //qDebug() << "Last Play DTime: " << stoq(cached_audio()->last_play_datetime()->valueToString());
           //qDebug() << "Cache Status : " << ac->is_dirty()->value();
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

           auto it_begin = data_provider->cache()->currentElement()->begin();
           auto it_end = data_provider->cache()->currentElement()->end();

           for (; it_begin != it_end; ++it_begin)
           {

                   auto [column, value] = (*it_begin);

                   if (column == "id")
                       ac->setId(str_to_int(value));

                   if (column == "cache_filepath")
                       ac->set_cache_filepath(value);

                   if (column == "cache_datetime")
                       ac->set_cache_datetime(QDateTime::fromString(QString::fromStdString(value)));

                   if (column == "last_play_datatime")
                       ac->set_last_play_datetime(QDateTime::fromString(QString::fromStdString(value)));

                   ac->set_is_cached(true);
           }

            m_cache.push_back(std::move(ac));

          data_provider->cache()->next();

        } while (!data_provider->cache()->isLast());


    }

    template<typename T>
    T* AudioCacheManager<T>::db_manager()
    {
        return m_dbmanager.get();
    }

    template<typename T>
    std::string AudioCacheManager<T>::get_cached_audio_path(int audio_id)
    {
        for (auto const& cache_audio: m_cache)
        {
            if (cache_audio->audio_id()->value() == audio_id){
                return cache_audio->cache_filepath()->value();
            }
        }
        return "";
    }

    template<typename T>
    void AudioCacheManager<T>::set_cache_audio_last_play_dtime(int audio_id)
    {
       for (auto const& cache_audio: m_cache){
           if (cache_audio->audio_id()->value() == audio_id){
               cache_audio->set_last_play_datetime(QDateTime::currentDateTime());
               cache_audio->set_is_dirty(true);
               break;
           }
       }
    }

    template<typename T>
    void AudioCacheManager<T>::persist_cache()
    {
        for (auto const& cache_audio : m_cache){
            if (cache_audio->is_dirty()->value()){
                m_dbmanager->updateEntity(*(cache_audio.get()));
                cache_audio->set_is_dirty(false);
            }
        }

    }

}

#endif // AUDIOCACHEMANAGER_H
