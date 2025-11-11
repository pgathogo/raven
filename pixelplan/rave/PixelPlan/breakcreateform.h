#ifndef BREAKCREATEFORM_H
#define BREAKCREATEFORM_H

#include <memory>
#include <QDialog>
#include <type_traits>

namespace Ui {
class BreakCreateForm;
}

class EntityDataModel;
class BreakLayoutLine;
class BreakLayout;
class BreakLayoutForm;
class Schedule;

using ScheduleDate = QDate;
using ScheduleHour = int;
using ScheduleTime = std::vector<QString>;
using ScheduleRecords =  std::map<ScheduleDate, std::map<ScheduleHour, ScheduleTime>>;


class BreakCreateForm : public QDialog
{
    Q_OBJECT

public:
    explicit BreakCreateForm(QWidget *parent = nullptr);
    ~BreakCreateForm();

    void populate_hour_combo();
    void save_break_layout_lines(std::shared_ptr<BreakLayoutForm>, int);


    template<typename T>
    struct Vectored{
        //Vectored():vec{}{}
        Vectored& operator<<(T* t)
        {
            vec.push_back(t);
            return *this;
        }
        void clear(){
            vec.clear();
        }
        std::vector<T*> vec;
    };

    template<typename E>
    constexpr auto to_underlying(E e) noexcept
    {
        return static_cast<std::underlying_type_t<E>>(e);
    }

    template<typename E>
    constexpr typename std::underlying_type<E>::type to_under(E e) noexcept {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }

private slots:
    void break_layout_selected(const QModelIndex& index);
    void close_form();
    void create_breaks();

    void add_hour();
    void remove_hour();

    void create_layout();
    void edit_layout();
    void delete_layout();

private:
    void set_defaults();
    void setup_ui();
    void make_progids_exclusion_list();
    void print_model_items();

    Ui::BreakCreateForm *ui;
    std::unique_ptr<EntityDataModel> m_edm_break_layout;
    std::unique_ptr<EntityDataModel> m_edm_break_line;
    std::shared_ptr<BreakLayout> m_break_layout;

    bool m_breaks_created{false};
    bool write_breaks_to_db(const std::string sql);
    std::string make_insert_statements(QDate from, QDate to);
    void get_existing_schedules(ScheduleRecords&, QDate, QDate);

    std::shared_ptr<BreakLayout> m_selected_breaklayout;
    std::vector<int> m_excluded_progids; // IDs of TVPrograms


};

#endif // BREAKCREATEFORM_H
