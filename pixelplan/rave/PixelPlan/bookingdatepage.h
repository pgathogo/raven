#ifndef BOOKINGDATEPAGE_H
#define BOOKINGDATEPAGE_H

#include <map>

#include <QWizardPage>

class QDateEdit;
class QListWidget;
class QListWidgetItem;
class QTableWidget;
class QTableWidgetItem;
class QCheckBox;

using progid = int;

namespace PIXELPLAN
{
    class OrderBookingWizard;

    struct ProgramBreak {
        int line_id = 0;
        QString break_time = "";
        int break_hour = 0;
        int duration   = 0;
        int max_spots  = 0;
        QString fill_method = "";
        QString dow = "";
        int progid = 0;
    };

    struct Program {
        QString title = "";
        std::vector<ProgramBreak> breaks;
    };


    class BookingDatePage : public QWizardPage
    {
        Q_OBJECT

    public:
        BookingDatePage(OrderBookingWizard*);
        bool validatePage() override;
        bool is_all_breaks();

        inline static QString name{"booking_date_page"};

    private slots:
        void on_program_clicked(QListWidgetItem*);
        void on_item_clicked(QTableWidgetItem*);
        void select_all_programs();
        void select_all_breaks(bool);

    private:
        void setup_ui();
        void get_tv_programs(std::map<progid, Program>&);
        void show_tv_programs(const std::map<progid, Program>&);
        void set_breaks_table(QTableWidget*);
        void show_program_breaks(std::vector<ProgramBreak>&);
        void fetch_selected_program_breaks();
        bool breaks_selected();
        void show_selection_count();

        // std::shared_ptr<BookingContext> m_booking_context;
        OrderBookingWizard* m_wizard;

        QDateEdit* m_de_start_date;
        QDateEdit* m_de_end_date;
        QListWidget* m_lw_programs;
        QTableWidget* m_tw_breaks;
        QLabel* m_lbl_selected_break_count;
        QCheckBox* m_cb_all_programs;
        QCheckBox* m_cb_all_breaks;

        std::map<progid, Program> m_programs;
    };
}


#endif