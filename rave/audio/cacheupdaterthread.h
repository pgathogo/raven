#ifndef CACHEUPDATERTHREAD_H
#define CACHEUPDATERTHREAD_H

#include <QThread>
#include <QObject>
#include <QMutex>

#include "audiocachemanager.h"

namespace AUDIO {

  template<typename T>
  class CacheUpdaterThread : public QThread
  {
  public:
      explicit CacheUpdaterThread(std::shared_ptr<AudioCacheManager<T>> cache_manager,
                                  std::shared_ptr<QMutex>,
                                  QObject* parent = nullptr);
      void run() override;

  private:
      std::shared_ptr<AudioCacheManager<T>> m_cache_manager;
      std::shared_ptr<QMutex> m_mutex;
  };

  template<typename T>
  CacheUpdaterThread<T>::CacheUpdaterThread(std::shared_ptr<AudioCacheManager<T>> cache_manager,
                                            std::shared_ptr<QMutex> mutex,
                                            QObject* parent)
      :m_mutex{mutex}
      ,m_cache_manager{cache_manager}
  {

  }

  template<typename T>
  void CacheUpdaterThread<T>::run()
  {
      m_mutex->try_lock();

      m_cache_manager->persist_cache();

      m_mutex->unlock();
  }

}

#endif // CACHEUPDATERTHREAD_H
