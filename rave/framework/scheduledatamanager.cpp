#include "scheduledatamanager.h"

namespace CORE{

    ScheduleDataManager::ScheduleDataManager()
    {
        auto m_edm = std::make_unique<EntityDataModel>();
    }

    ScheduleDataManager::~ScheduleDataManager()
    {
    }

    void ScheduleDataManager::fetch_db_schedule(QDate, int)
    {
    }

    void ScheduleDataManager::fetch_local_schedule(QDate, int)
    {
    }

    void ScheduleDataManager::add_schedule_item()
    {
    }

    void ScheduleDataManager::delete_schedule_item()
    {
    }

    std::size_t ScheduleDataManager::size() const
    {
        return m_schedule_items.size();
    }
}
