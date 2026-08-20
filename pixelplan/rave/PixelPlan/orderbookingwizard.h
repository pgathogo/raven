#ifndef ORDERBOOKINGWIZARD_H
#define ORDERBOOKINGWIZARD_H

#include <type_traits>
#include <functional>

#include <QWizard>
#include <QDate>

#include "spotspage.h"
#include "traffikrules.h"

class Order;

namespace TRAFFIK {
class Spot;
}

namespace PIXELPLAN
{
    //class RuleEngine;
    //class EngineData;


    struct SelectedBreak{
        int break_id;
        QDate break_date;
        QTime break_time;
        int break_hour;
        int booked_spots;
        int max_spots;
        std::string break_fill_method;
    };

    struct BookingData {
        std::shared_ptr<TRAFFIK::Spot> spot{nullptr};
        bool is_all_breaks{false};
        QDate start_date;
        QDate end_date;
        std::vector<SelectedProgramBreak> selected_breaks{};
        // std::unique_ptr<PIXELPLAN::RuleEngine> m_rule_engine;
        std::shared_ptr<PIXELPLAN::EngineData> m_engine_data{nullptr};
        std::map<int, SelectedBreak> final_selected_breaks;
    };

    class OrderBookingWizard : public QWizard
    {
        Q_OBJECT

    public:
        explicit OrderBookingWizard(const std::string&, Order*, QWidget* parent=nullptr);

        ~OrderBookingWizard();


        template<typename T, typename... TArgs>
        T* register_page(TArgs&&... args) {

            static_assert( requires(T& page) {
                page.name;
            }, " T must have a field named `name`");

            T* page =  new T(std::forward<TArgs>(args)...);

            m_pages[page->name] = page;

            return page;
        }

        BookingData booking_data;

        void init_rules_state();

    private:

        std::map<QString, QWizardPage*> m_pages;
        std::map<QString, std::function<std::unique_ptr<QWizardPage>()>> m_page_types;

    };
}



#endif

