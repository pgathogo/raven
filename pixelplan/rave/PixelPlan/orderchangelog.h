#pragma once

#include "../../../rave/framework/baseentity.h"

namespace  PIXELPLAN {

class OrderChangeLog : public BaseEntity{

public:
    OrderChangeLog();

    StringField* order_number() const;
    ForeignKeyField* order() const;
    ForeignKeyField* client() const;
    IntegerField* seq() const;
    DateField* change_date() const;
    DateField* prev_order_date() const;
    DateField* prev_start_date() const;
    DateField* prev_end_date() const;
    IntegerField* prev_spots_ordered() const;
    IntegerField* prev_spots_booked() const;
    IntegerField* prev_spots_played() const;
    TextField* prev_comment() const;
    DateField* new_start_date() const;
    DateField* new_end_date() const;
    IntegerField* new_spots_ordered() const;
    TextField* new_comment() const;
    StringField* add_login() const;
    DateTimeField* add_dtime() const;

    void set_order_number(const std::string);
    void set_order(int);
    void set_client(int);
    void set_seq(int);
    void set_change_date(QDate);
    void set_prev_order_date(QDate);
    void set_prev_start_date(QDate);
    void set_prev_end_date(QDate);
    void set_prev_spots_ordered(int);
    void set_prev_spots_booked(int);
    void set_prev_spots_played(int);
    void set_prev_comment(const std::string);
    void set_new_start_date(QDate);
    void set_new_end_date(QDate);
    void set_new_spots_ordered(int);
    void set_new_comment(const std::string);
    void set_add_login(const std::string);
    void set_add_dtime(QDateTime);

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::shared_ptr<BaseEntity> cloneAsShared() override;
    void afterMapping(BaseEntity& entity) override;

    [[nodiscard]] ActionResult validate() override;
private:
    StringField* m_order_number;
    ForeignKeyField* m_order;
    ForeignKeyField* m_client;
    IntegerField* m_seq;
    DateField* m_change_date;
    DateField* m_prev_order_date;
    DateField* m_prev_start_date;
    DateField* m_prev_end_date;
    IntegerField* m_prev_spots_ordered;
    IntegerField* m_prev_spots_booked;
    IntegerField* m_prev_spots_played;
    TextField* m_prev_comment;
    DateField* m_new_start_date;
    DateField* m_new_end_date;
    IntegerField* m_new_spots_ordered;
    TextField* m_new_comment;
    StringField* m_add_login;
    DateTimeField* m_add_dtime;

    QStringList mHeader;
    std::string m_tablename;

};



}