#ifndef BOOKINGORDERBROWSER_H
#define BOOKINGORDERBROWSER_H

#include <memory>
#include <QDialog>
#include <QItemDelegate>
#include <QMenu>
#include <QCompleter>
#include "../framework/picklistbrowser.h"


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
    int schedule_id{-1};
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

    void search_by_client(Client* client);

private slots:
    void search_field_changed(int i);
    void search(int);
    void select_filter();
    void clear_filter();
    void cancel_query();
    void new_booking();
    void show_spot_details(const QPoint& pos);
    void spot_details(int);
    void find_orders(QString);

private:
    Ui::BookingOrderBrowser *ui;
    QMdiArea* m_mdi_area;
    std::unique_ptr<PickListBrowser> m_picklist_browser;

    int m_id{-1};
    std::string m_label{};

    std::string make_filter(int);
    std::string order_by(int);

    std::vector<QTableWidget*> m_grid_tables;
    std::vector<int> booking_ids;

    std::vector<QTreeWidgetItem*> m_tree_nodes;

    TreeWidgetItemDelegate m_item_delegate;

    QMenu* m_spot_ctx_menu;
    QAction* m_spot_ctx_action;

    void set_treewidget(Bookings&, int);
    void resizeColumnsToContents(QTreeWidget& tree_widget);
    void sort_bookings(Bookings& orders);
    void cancel_booking();
    void make_spot_menu();
    QTableWidget* get_selected_grid();

    void set_autocompleter();

    QCompleter* m_completer;
    std::unique_ptr<EntityDataModel> m_client_edm;

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
