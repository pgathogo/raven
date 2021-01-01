#ifndef BOOKINGORDERBROWSER_H
#define BOOKINGORDERBROWSER_H

#include <memory>
#include <QDialog>

namespace Ui {
class BookingOrderBrowser;
}

class QMdiArea;
class QTreeWidget;
class PickListBrowser;

struct Booking{
    std::string client_name{};
    std::string order_title{};
    int order_id{-1};
    int order_number{-1};
    std::string order_date{-1};
    std::string start_date{-1};
    std::string end_date{-1};
    int spots_ordered{-1};
    int spots_booked{-1};
    int spot_id{-1};
    std::string spot_name{};
    double spot_duration{0};
    int booking_id{-1};
    std::string booking_status{};
    std::string play_date{};
    std::string play_time{};
    std::string schedule_date{};
    std::string schedule_time{};
};

using Bookings = std::map<int, std::vector<Booking>>;

class BookingOrderBrowser : public QDialog
{
    Q_OBJECT

public:
    enum FilterField{Client_Name_Field=0, Order_Title_Field, Booking_Period_Field};

    explicit BookingOrderBrowser(QWidget *parent = nullptr);
    ~BookingOrderBrowser();

    void setMdiArea(QMdiArea* mdi);

private slots:
    void search_field_changed(int i);
    void search();
    void select_filter();

private:
    Ui::BookingOrderBrowser *ui;
    QMdiArea* m_mdi_area;
    std::unique_ptr<PickListBrowser> m_picklist_browser;

    int m_client_id;
    int m_order_id;

    std::string make_filter();
    void set_treewidget(Bookings& records);
    void resizeColumnsToContents(QTreeWidget& tree_widget);

    void filter_by_client();
    void filter_by_order();
};

#endif // BOOKINGORDERBROWSER_H
