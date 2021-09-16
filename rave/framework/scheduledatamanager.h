#pragma once

#include "entitydatamodel.h"

class Schedule;

namespace CORE{

    class ScheduleDataManager
    {
    public:
        enum class ScheduleTransition{None=0, Mix, Cut, Center, Early, Syncro, Stop };

        ScheduleDataManager();
        ~ScheduleDataManager();

        void fetch_db_schedule(QDate, int);
        void fetch_local_schedule(QDate, int);

        void add_schedule_item();
        void delete_schedule_item();

        std::size_t size() const;
    private:
        std::vector<Schedule*> m_schedule_items;
        std::unique_ptr<EntityDataModel> m_edm;

    };

}
