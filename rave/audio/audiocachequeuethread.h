#ifndef QUEUECACHEWORKER_H
#define QUEUECACHEWORKER_H

#include <QThread>
#include <QObject>
#include <QMutex>
#include <vector>

#include "audiocache.h"
#include "audiocachemanager.h"

namespace AUDIO {

    enum class ThreadStatus{READY, BUSY};

    template<typename T>
    class AudioCacheQueueThread : public QThread
    {
    public:

        explicit AudioCacheQueueThread(std::shared_ptr<AudioCacheManager<T>> cache_manager,
                                       std::shared_ptr<QMutex> mutex,
                                       QObject* parent = nullptr);

        void run() override;
        void queue_add(std::shared_ptr<AudioCache>);
        ThreadStatus thread_status();
        int thread_id();
    private:
        int m_thread_id{-1};
        std::vector<std::unique_ptr<AudioCache>> m_queue;
        std::shared_ptr<QMutex> m_mutex;
        std::shared_ptr<AudioCacheManager<T>> m_cache_manager;
        ThreadStatus m_thread_status;
        static int thread_seq;
    };

    template<typename T>
    int AudioCacheQueueThread<T>::thread_seq{0};

    template<typename T>
    AudioCacheQueueThread<T>::AudioCacheQueueThread(std::shared_ptr<AudioCacheManager<T>> cache_manager,
                                                    std::shared_ptr<QMutex> mutex,
                                                    QObject* parent)
        : m_mutex{mutex}
        ,m_cache_manager{cache_manager}
        ,m_thread_status{ThreadStatus::READY}
    {
        m_thread_id = thread_seq++;
    }

    template<typename T>
    void AudioCacheQueueThread<T>::run()
    {
        m_mutex->try_lock();

        m_thread_status = ThreadStatus::BUSY;

        m_cache_manager->cache_audio(); // From queue to main cache

        m_mutex->unlock();

        m_thread_status = ThreadStatus::READY;

    }

    template<typename T>
    ThreadStatus AudioCacheQueueThread<T>::thread_status()
    {
        return m_thread_status;
    }

    template<typename T>
    int AudioCacheQueueThread<T>::thread_id()
    {
        return m_thread_id;
    }

    template<typename T>
    void AudioCacheQueueThread<T>::queue_add(std::shared_ptr<AudioCache> audio)
    {
        m_mutex->try_lock();

        m_thread_status = ThreadStatus::BUSY;
        m_cache_manager->queue_cache(std::move(audio));

        m_mutex->unlock();

        m_thread_status = ThreadStatus::READY;
    }

}
#endif // QUEUECACHEWORKER_H
