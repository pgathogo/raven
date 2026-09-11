#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QGridLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

#include "../../../rave/framework/entitydatamodel.h"
#include "breakselectionpage.h"
#include "orderbookingwizard.h"
#include "schedule.h"
#include "order.h"
#include "traffikrules.h"

namespace PIXELPLAN
{


BreakSelectionPage::BreakSelectionPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Break Selection");
    setSubTitle("Select breaks to place booking");

   setup_ui();
}

void  BreakSelectionPage::setup_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout();

    QHBoxLayout* hl_sel_all = new QHBoxLayout();
    m_cb_all = new QCheckBox("Select All");
    connect(m_cb_all, &QCheckBox::toggled, this, &BreakSelectionPage::on_state_changed);

    hl_sel_all->addWidget(m_cb_all);
    hl_sel_all->addStretch();

    main_layout->addLayout(hl_sel_all);

    QHBoxLayout* hl_items = new QHBoxLayout();

    m_tw_break_select = new QTableWidget();
    m_tw_break_select->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tw_break_select->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tw_break_select->setCornerButtonEnabled(true);
    connect(m_tw_break_select, &QTableWidget::itemClicked, this, &BreakSelectionPage::on_item_clicked);

    hl_items->addWidget(m_tw_break_select);

    QVBoxLayout* vl_grid = new QVBoxLayout();
    hl_items->addLayout(vl_grid);

    // Top Grid - Available Breaks

    QGridLayout* gl_top = new QGridLayout();
    QLabel* lbl_avail = new QLabel("Available Breaks:");
    m_lbl_avail_val = new QLabel("0");

    QLabel* lbl_sel = new QLabel("Selected Breaks:");
    m_lbl_sel_val = new QLabel("0");

    gl_top->addWidget(lbl_avail, 0, 0);
    gl_top->addWidget(m_lbl_avail_val, 0, 1);
    gl_top->addWidget(lbl_sel, 1, 0);
    gl_top->addWidget(m_lbl_sel_val, 1, 1);

    vl_grid->addLayout(gl_top);

    // Bottom Grid - Order Details

    QGridLayout* gl_bottom = new QGridLayout();
    QLabel* lbl_order = new QLabel("Order Number:");
    m_lbl_order_val = new QLabel("0");

    QLabel* lbl_spots_ordered = new QLabel("Spots Ordered:");
    m_lbl_spots_ordered_val = new QLabel("0");

    QLabel* lbl_spots_booked = new QLabel("Spots Booked");
    m_lbl_spots_booked_val = new QLabel("0");

    QLabel* lbl_spots_pending = new QLabel("Spots Pending:");
    QLabel* lbl_spots_pending_val = new QLabel("0");

    gl_bottom->addWidget(lbl_order, 0, 0);
    gl_bottom->addWidget(m_lbl_order_val, 0, 1);

    gl_bottom->addWidget(lbl_spots_ordered, 1, 0);
    gl_bottom->addWidget(m_lbl_spots_ordered_val, 1, 1);

    gl_bottom->addWidget(lbl_spots_booked, 2, 0);
    gl_bottom->addWidget(m_lbl_spots_booked_val, 2, 1);

    gl_bottom->addWidget(lbl_spots_pending, 3, 0);
    gl_bottom->addWidget(lbl_spots_pending_val, 3, 1);

    vl_grid->addSpacing(100);

    vl_grid->addLayout(gl_bottom);

    main_layout->addLayout(hl_items);

    setLayout(main_layout);

}

void BreakSelectionPage::initializePage()
{
    setup_break_select_grid();
    m_cb_all->setChecked(true);

    m_lbl_avail_val->setText(QString::number(m_wizard->booking_data.m_engine_data->available_breaks));
    m_lbl_order_val->setText(m_wizard->booking_data.order->orderNumber()->to_qstring());
    m_lbl_spots_ordered_val->setText(QString::number(m_wizard->booking_data.order->spotsOrdered()->value()));
    m_lbl_spots_booked_val->setText(QString::number(m_wizard->booking_data.order->spotsBooked()->value()));

}

bool BreakSelectionPage::validatePage()
{
    if (m_tw_break_select->selectionModel()->selectedRows().count() == 0) {
        showMessage("No breaks selected!", QMessageBox::Critical);
        return false;
    }

    m_wizard->booking_data.sel_breaks = m_tw_break_select->selectedItems();
    m_wizard->booking_data.sel_break_count = m_tw_break_select->selectionModel()->selectedRows().count();

    return true;
}


void BreakSelectionPage::setup_break_select_grid()
{

    QStringList header_labels;
    header_labels << "Date" << "Time" << "Remaining Time (secs)";

    m_tw_break_select->setColumnCount(3);
    m_tw_break_select->setHorizontalHeaderLabels(header_labels);
    m_tw_break_select->setRowCount(m_wizard->booking_data.m_engine_data->available_breaks);
    m_tw_break_select->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;

    constexpr int DATE_COL = 0;
    constexpr int TIME_COL = 1;
    constexpr int DURATION_COL = 2;

    std::vector<SelectedBreak> selected_breaks;

    for (auto& [name, entity] : m_wizard->booking_data.m_engine_data->m_schedule_EDM->modelEntities())
    {
        Schedule* comm_break = dynamic_cast<Schedule*>(entity.get());

        if (comm_break->break_availability() == Schedule::BreakAvailability::Break_Not_Available)
            continue;

        QString date_str = comm_break->schedule_date()->value().toString();
        QString time_str = comm_break->schedule_time()->value().toString("HH:mm");
        QString dur_str = QString::number(comm_break->break_duration_left()->value());

        QTableWidgetItem* date_item = new QTableWidgetItem(date_str);
        date_item->setData(Qt::UserRole, comm_break->id());

        m_tw_break_select->setItem(row, DATE_COL, date_item);

        QTableWidgetItem* time_item = new QTableWidgetItem(time_str);
        m_tw_break_select->setItem(row, TIME_COL, time_item);
        time_item->setTextAlignment(Qt::AlignCenter);

        int int_date = comm_break->schedule_date()->value().day()+
            comm_break->schedule_date()->value().month()+
            comm_break->schedule_date()->value().year();
            time_item->setData(Qt::UserRole, int_date);

        QTableWidgetItem* dur_item = new QTableWidgetItem(dur_str);
        m_tw_break_select->setItem(row, DURATION_COL, dur_item);
        dur_item->setTextAlignment(Qt::AlignCenter);
        dur_item->setData(Qt::UserRole, comm_break->schedule_date()->value().dayOfWeek());

        SelectedBreak sel_break;

        sel_break.break_id = comm_break->id();
        sel_break.break_date = comm_break->schedule_date()->value();
        sel_break.break_time = comm_break->schedule_time()->value();
        sel_break.break_hour = comm_break->schedule_hour()->value();
        sel_break.booked_spots = comm_break->booked_spots()->value();
        sel_break.break_fill_method = comm_break->break_fill_method()->value();
        sel_break.max_spots = comm_break->break_max_spots()->value();

        selected_breaks.push_back(sel_break);

        ++row;
    }

    std::sort(selected_breaks.begin(), selected_breaks.end(), [](const SelectedBreak& a, const SelectedBreak& b){
        if (a.break_date == b.break_date)
            return a.break_time < b.break_time;
        return a.break_date < b.break_date;
    });

    for (const auto& sel_break : selected_breaks)
    {
        m_wizard->booking_data.final_selected_breaks[sel_break.break_id] = sel_break;
    }
}

void BreakSelectionPage::on_state_changed(int state)
{

    if (m_cb_all->isChecked()) {

        m_tw_break_select->selectAll();

    } else {
        m_tw_break_select->clearSelection();
    }

    show_selection_count();
}

void BreakSelectionPage::on_item_clicked(QTableWidgetItem* item)
{

    show_selection_count();

}

void BreakSelectionPage::show_selection_count()
{
    int sel_count = m_tw_break_select->selectionModel()->selectedRows().count();
    m_lbl_sel_val->setText(QString::number(sel_count));
}

} // namespace