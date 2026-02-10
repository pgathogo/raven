#ifndef BOOKINGORDERBROWSER_H
#define BOOKINGORDERBROWSER_H

#include <memory>
#include <QDialog>
#include <QItemDelegate>
#include <QMenu>
#include <QCompleter>
#include <QTableWidgetItem>
#include "../../../rave/framework/picklistbrowser.h"


namespace Ui {
class BookingOrderBrowser;
}

class QMdiArea;
class QTreeWidget;
class QTableWidget;
class QTreeWidgetItem;
class PickListBrowser;
class Client;


class TreeWidgetItemDelegate : public QItemDelegate
{
public:
    TreeWidgetItemDelegate(QObject* poParent = nullptr, int height = -1):
        QItemDelegate(poParent), m_row_height{height}{}

    void setHeight(int height)
    {
        m_row_height = height;
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QSize o_size = QItemDelegate::sizeHint(option, index);

        if (m_row_height != -1){
            o_size.setHeight(m_row_height);
        }

        return o_size;
    }

private:
    int m_row_height;
};

struct Booking{
    std::string client_name{};
    std::string order_title{};
    int order_id{-1};
    std::string order_number{};
    std::string order_date{-1};
    std::string start_date{-1};
    std::string end_date{-1};
    int spots_ordered{-1};
    int spots_booked{-1};
    int spot_id{-1};
    std::string spot_name{};
    double spot_duration{0};
    QString formatted_duration{};
    int booking_id{-1};
    std::string booking_status{};
    std::string play_date{};
    std::string play_time{};
    int schedule_id{-1};
    std::string schedule_date{};
    std::string schedule_time{};
};

// NOTE: We call this struct 'ClientOrder' to avoid confusing it with the 'Order' Entity
struct ClientOrder {
    int order_id{0};
    std::string order_number{""};
    std::string title{""};
    QDate order_date;
    QDate start_date;
    QDate end_date;
    int spots_ordered{0};
    int spots_booked{0};
    int client_id{0};
    std::string client_name{""};
    std::vector<Booking> order_bookings;
};

struct VoidReason {
    int reason_id;
    std::string other_reason;
    std::string void_type;
    std::string username;
};

class BookingItem
{
public:
    BookingItem(Booking bk);

    void set_font_color(QColor c);

    QTableWidgetItem* book_date();
    QTableWidgetItem* book_time();
    QTableWidgetItem* spot_title();
    QTableWidgetItem* spot_duration();
    QTableWidgetItem* tx_date();
    QTableWidgetItem* tx_time();
    QTableWidgetItem* book_status();

private:
    QTableWidgetItem* m_book_date = nullptr;
    QTableWidgetItem* m_book_time = nullptr;
    QTableWidgetItem* m_spot_title = nullptr;
    QTableWidgetItem* m_spot_duration = nullptr;
    QTableWidgetItem* m_tx_date = nullptr;
    QTableWidgetItem* m_tx_time = nullptr;
    QTableWidgetItem* m_book_status = nullptr;
};


using Bookings = std::map<int, std::vector<Booking>>;

enum class VoidType{Cancel, Skip};

class BookingOrderBrowser : public QDialog
{
    Q_OBJECT

public:
    enum FilterField{Client_Name_Field=0, Order_Title_Field, Booking_Period_Field};


    explicit BookingOrderBrowser(const std::string, QWidget *parent = nullptr);
    ~BookingOrderBrowser();

    void setMdiArea(QMdiArea* mdi);

    void search_by_client(std::shared_ptr<Client>);
    BookingItem make_booking_item(Booking);

private slots:
    void search_field_changed(int i);
    void search(int);
    void select_filter();
    void clear_filter();
    void cancel_clicked();
    void skip_clicked();
    void new_booking();
    void show_spot_details(const QPoint& pos);
    void spot_details(int);
    void find_orders(QString);
    void date_filter_changed(int);

    void print_all_bookings();

private:
    void set_treewidget(Bookings&, int, const std::string);
    void resizeColumnsToContents(QTreeWidget& tree_widget);
    void sort_bookings(std::vector<Booking>&);
    void void_booking(int, std::vector<int>, VoidReason);
    void make_spot_menu();
    QTableWidget* get_selected_grid();
    void set_autocompleter();
    void fill_cbox_date_filter();
    void set_client(std::shared_ptr<Client>);
    void build_client_orders(int, std::string, std::vector<ClientOrder>&);
    void build_order_bookings(int, std::vector<Booking>&);
    void build_order_booking_table(std::vector<ClientOrder>&);
    void make_inner_table_headers(QTableWidget*, int);

    void void_query(VoidType);
    std::tuple<std::string, std::string> tag_n_type(VoidType);
    std::tuple<int, std::vector<int>> get_selected_bookings();

    std::string vector_to_comma_sep(const std::vector<int>&);



    Ui::BookingOrderBrowser *ui;
    QMdiArea* m_mdi_area;
    std::unique_ptr<PickListBrowser> m_picklist_browser;

    int m_id{-1};
    std::string m_label{};

    std::string make_filter(int);
    std::string make_order_date_filter();
    std::string order_by(int);

    std::vector<QTableWidget*> m_grid_tables;
    std::vector<int> booking_ids;

    std::vector<QTreeWidgetItem*> m_tree_nodes;

    TreeWidgetItemDelegate m_item_delegate;

    QMenu* m_spot_ctx_menu;
    QAction* m_spot_ctx_action;

    std::shared_ptr<Client> m_client;

    int t_row{-1};
    int t_col{-1};

    std::string m_username;


    QCompleter* m_completer;
    std::unique_ptr<EntityDataModel> m_client_edm;

    std::vector<ClientOrder> m_client_orders;

    template<typename T>
    class HasNameColumn
    {
        private:
            typedef char YesType[1];
            typedef char NoType[2];

            template <typename C>
            static YesType& test(decltype(&C::name));
            template <typename C>
            static NoType& test(...);
        public:
            enum{ value = sizeof(test<T>(0)) == sizeof(YesType) };
    };


    template <typename T>
    std::tuple<int, std::string> filter_query()
    {
        PickListSetting set(std::make_unique<T>(), 0);
        m_picklist_browser = std::make_unique<PickListBrowser>(set);
        auto results = m_picklist_browser->exec();

        if (results == 0)
            return std::make_tuple(-1,"");

        for (const auto& base_entity : set.selectedEntities){
            auto entity = dynamic_cast<T*>(base_entity);
            if constexpr(HasNameColumn<T>::value > 0)
                    return std::make_tuple(entity->id(), entity->name()->displayName());
            else
                return std::make_tuple(entity->id(), entity->title()->displayName());
        }
    }


};

class PrintBookingMenu : public QMenu
{
    Q_OBJECT
public:
    explicit PrintBookingMenu(QPushButton* button, QWidget* parent=0);
    void showEvent(QShowEvent* event);
private:
    QPushButton* m_push_button;
};


#endif // BOOKINGORDERBROWSER_H
