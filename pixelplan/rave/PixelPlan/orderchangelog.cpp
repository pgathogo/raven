#include "order.h"
#include "client.h"
#include "orderchangelog.h"

namespace PIXELPLAN
{

    OrderChangeLog::OrderChangeLog()
    {
        m_order_number = createField<StringField>("order_number", "order_number");
        m_order = createField<ForeignKeyField>("order_id", "Order ID", std::make_unique<Order>(), "title");
        m_client = createField<ForeignKeyField>("client_id", "Client ID", std::make_unique<Client>(), "name");
        m_seq = createField<IntegerField>("seq", "Sequence Number");
        m_change_date = createField<DateField>("change_date", "change_date");
        m_prev_order_date = createField<DateField>("prev_order_date", "prev_order_date");
        m_prev_start_date = createField<DateField>("prev_start_date", "prev_start_date");
        m_prev_end_date = createField<DateField>("prev_end_date", "prev_end_date");
        m_prev_spots_ordered = createField<IntegerField>("prev_spots_ordered", "prev_spots_ordered");
        m_prev_spots_booked = createField<IntegerField>("prev_spots_booked", "prev_spots_booked");
        m_prev_spots_played = createField<IntegerField>("prev_spots_played", "prev_spots_played");
        m_prev_comment = createField<TextField>("prev_comment", "prev_comment");
        m_new_start_date = createField<DateField>("new_start_date", "new_start_date");
        m_new_end_date = createField<DateField>("new_end_date", "new_end_date");
        m_new_spots_ordered = createField<IntegerField>("new_spots_ordered", "new_spots_ordered");
        m_new_comment = createField<TextField>("new_comment", "new_comment");
        m_add_login = createField<StringField>("add_login", "add_login");
        m_add_dtime = createField<DateTimeField>("add_dtime", "add_dtime");

        setTableName("rave_orderchangelog");
    }

    StringField* OrderChangeLog::order_number() const { return m_order_number; }
    ForeignKeyField* OrderChangeLog::order() const {return m_order; }
    ForeignKeyField* OrderChangeLog::client() const {return m_client; }
    IntegerField* OrderChangeLog::seq() const {return m_seq; }
    DateField* OrderChangeLog::change_date() const { return m_change_date; }
    DateField* OrderChangeLog::prev_order_date() const { return m_prev_order_date; }
    DateField* OrderChangeLog::prev_start_date() const { return m_prev_start_date; }
    DateField* OrderChangeLog::prev_end_date() const { return m_prev_end_date; }
    IntegerField* OrderChangeLog::prev_spots_ordered() const { return m_prev_spots_ordered; }
    IntegerField* OrderChangeLog::prev_spots_booked() const { return m_prev_spots_booked; }
    IntegerField* OrderChangeLog::prev_spots_played() const { return m_prev_spots_played; }
    TextField* OrderChangeLog::prev_comment() const { return m_prev_comment; }
    DateField* OrderChangeLog::new_start_date() const { return m_new_start_date; }
    DateField* OrderChangeLog::new_end_date() const { return m_new_end_date; }
    IntegerField* OrderChangeLog::new_spots_ordered() const { return m_new_spots_ordered; }
    TextField* OrderChangeLog::new_comment() const { return m_new_comment; }
    StringField* OrderChangeLog::add_login() const { return m_add_login; }
    DateTimeField* OrderChangeLog::add_dtime() const { return m_add_dtime; }

    void OrderChangeLog::set_order_number(const std::string ord_num){ m_order_number->setValue(ord_num); }
    void OrderChangeLog::set_order(int id) { m_order->setValue(id); }
    void OrderChangeLog::set_client(int id) { m_client->setValue(id); }
    void OrderChangeLog::set_seq(int seq_no) {m_seq->setValue(seq_no); }
    void OrderChangeLog::set_change_date(QDate date){ m_change_date->setValue(date); }
    void OrderChangeLog::set_prev_order_date(QDate date){ m_prev_order_date->setValue(date); }
    void OrderChangeLog::set_prev_start_date(QDate date) { m_prev_start_date->setValue(date); }
    void OrderChangeLog::set_prev_end_date(QDate date) { m_prev_end_date->setValue(date); }
    void OrderChangeLog::set_prev_spots_ordered(int spots){ m_prev_spots_ordered->setValue(spots); }
    void OrderChangeLog::set_prev_spots_booked(int spots){ m_prev_spots_booked->setValue(spots); }
    void OrderChangeLog::set_prev_spots_played(int spots) { m_prev_spots_played->setValue(spots); }
    void OrderChangeLog::set_prev_comment(const std::string comment) { m_prev_comment->setValue(comment); }
    void OrderChangeLog::set_new_start_date(QDate sdate) { m_new_start_date->setValue(sdate); }
    void OrderChangeLog::set_new_end_date(QDate edate) { m_new_end_date->setValue(edate); }
    void OrderChangeLog::set_new_spots_ordered(int spots) { m_new_spots_ordered->setValue(spots); }
    void OrderChangeLog::set_new_comment(const std::string comment) { m_new_comment->setValue(comment); }
    void OrderChangeLog::set_add_login(const std::string userid) { m_add_login->setValue(userid); }
    void OrderChangeLog::set_add_dtime(QDateTime dtime) { m_add_dtime->setValue(dtime); }

    std::string OrderChangeLog::tableName() const{ return m_tablename; }
    void OrderChangeLog::setTableName(const std::string table_name) { m_tablename = table_name; }

    std::unique_ptr<BaseEntity> OrderChangeLog::mapFields(StringMap* raw_entity) {}

    std::vector<std::string> OrderChangeLog::tableViewColumns() const
    {
        return {order_number()->displayName()};
    }

    std::vector<std::string> OrderChangeLog::tableViewValues() {
        return { order_number()->displayName()};
    }

    QStringList OrderChangeLog::tableHeaders() const { return mHeader; }

    std::string OrderChangeLog::searchColumn() const { return order_number()->valueToString(); }
    void OrderChangeLog::populateEntity() {}

    std::shared_ptr<BaseEntity> OrderChangeLog::cloneAsShared() { return std::make_shared<OrderChangeLog>(); }
    void OrderChangeLog::afterMapping(BaseEntity& entity) {}

    ActionResult OrderChangeLog::validate() {return BaseEntity::validate(); }
}
