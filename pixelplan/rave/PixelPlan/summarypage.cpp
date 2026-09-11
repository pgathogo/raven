#include <random>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeView>

#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/framework/entitydatamodel.h"

#include "bookingsegment.h"
#include "order.h"
#include "orderbookingwizard.h"
#include "orderbooking.h"
#include "spot.h"
#include "summarypage.h"
#include "traffiktreeviewmodel.h"

namespace PIXELPLAN
{

SummaryPage::SummaryPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Summary");
    setSubTitle("Verify break selection is okay before confirming the booking");

    setup_ui();
}

void SummaryPage::setup_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout();

    m_tv_summary = new QTreeView();

    QHBoxLayout* hl_summary = new QHBoxLayout();
    QLabel* lbl_total = new QLabel("Total Bookings:");
    QLabel* lbl_total_val = new QLabel("0");
    hl_summary->addWidget(lbl_total);
    hl_summary->addWidget(lbl_total_val);
    hl_summary->addStretch();

    main_layout->addWidget(m_tv_summary);
    main_layout->addLayout(hl_summary);

    setLayout(main_layout);

}

void SummaryPage::initializePage()
{
    TRAFFIK::TraffikTreeViewModel* tvm  = new TRAFFIK::TraffikTreeViewModel(m_wizard->booking_data.sel_breaks);
    m_tv_summary->setModel(tvm);
    m_tv_summary->expandAll();

    qDebug() << "Oder ID: " << m_wizard->booking_data.order->id();
}

bool SummaryPage::validatePage()
{
    bool booking_results = make_booking();
    return booking_results;
}

bool SummaryPage::make_booking()
{
    if (QMessageBox::question(this, tr("Traffik"),
                                tr("Are you sure you want to commit the booking(s)?"),
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes){
        //print_selected_breaks();
        commit_booking();
        QMessageBox::information(this, tr("Traffik"),
                                 tr("Booking committed successfully."),QMessageBox::Ok);
        return true;
    }

    return false;

}

void SummaryPage::commit_booking()
{
    try{

        BookingSegment book_segment;

        book_segment.set_booking_date(QDate::currentDate());

        book_segment.set_start_date(m_wizard->booking_data.start_date);
        book_segment.set_end_date(m_wizard->booking_data.end_date);

        book_segment.set_booking_count(m_wizard->booking_data.sel_break_count);

        book_segment.set_segment_value(0);
        book_segment.set_comments("Comm Booking");

        book_segment.set_order(m_wizard->booking_data.order->id());

        book_segment.set_add_login(m_wizard->booking_data.username.toStdString());

        book_segment.set_add_date_time(QDateTime::currentDateTime());

        EntityDataModel edm;

        auto selected_breaks = m_wizard->booking_data.sel_breaks;

        int book_segment_id = edm.createEntityDB(book_segment);

        for (int i=0; i < selected_breaks.count(); ++i)
        {
            auto item = selected_breaks.at(i);

            if (item->column() == 0)
            {
                OrderBooking order_booking;

                int break_id = item->data(Qt::UserRole).toInt();

                SelectedBreak selected_break = m_wizard->booking_data.final_selected_breaks[break_id];

                order_booking.set_booking_status("READY");
                order_booking.set_schedule(break_id);

                order_booking.set_spot(m_wizard->booking_data.m_engine_data->spot_to_book.spot_id);

                order_booking.set_booking_segment(book_segment_id);
                order_booking.play_date()->setReadOnly(true);
                order_booking.play_time()->setReadOnly(true);

                order_booking.set_book_date(selected_break.break_date);
                order_booking.set_book_time(selected_break.break_time);
                order_booking.set_book_hour(selected_break.break_hour);

                order_booking.set_audio(m_wizard->booking_data.m_engine_data->spot_to_book.audio_id);

                //order_booking.set_order(m_order->id());
                order_booking.set_order(m_wizard->booking_data.order->id());

                order_booking.set_add_dtime(QDateTime::currentDateTime());
                //order_booking.set_add_login(m_username);
                order_booking.set_add_login(m_wizard->booking_data.username.toStdString());
                order_booking.set_void_reason(-1);

                // Sequential slotting
                if (selected_break.break_fill_method == "S") {
                    order_booking.set_book_seq(++selected_break.booked_spots);
                }

                // Random slotting
                if (selected_break.break_fill_method == "R") {
                    int break_slot = find_break_slot(break_id, selected_break.max_spots);
                    order_booking.set_book_seq(break_slot);
                }

                qDebug() << "1. Create Order Booking...";

                edm.createEntityDB(order_booking);

                qDebug() << "2. Order booking created**";

                // Deduct time remainining on this break
                std::stringstream  sql ;
                sql << "Update rave_schedule set break_duration_left =  break_duration_left - "
                    << std::to_string(m_wizard->booking_data.m_engine_data->spot_to_book.spot_duration)
                    << ", booked_spots = booked_spots + 1 "
                    << " Where id = "+std::to_string(break_id);

                std::cout << sql.str() << '\n';

                qDebug() << "3. Updating rave_schedule break_duration_left...";

                edm.executeRawSQL(sql.str());

                qDebug() << "4. rave_schedule update done**";


            }
        }


        std::stringstream update_order;
        update_order << "Update rave_order set spots_booked = spots_booked + "
                     << std::to_string(book_segment.booking_count()->value())
                     << " Where id = "
                     << std::to_string(m_wizard->booking_data.order->id());
                     //<< " Where order_number = '"+m_order->orderNumber()->value()+"'";

        std::cout << update_order.str() << '\n';

        qDebug()  << "5. Updating rave_order...";

        edm.executeRawSQL(update_order.str());

        qDebug() << "* End *";

    }

    catch(DatabaseException& de){
        showMessage(de.errorMessage());
    }

}

int SummaryPage::find_break_slot(int break_id, int max_spots)
{
    std::vector<int> prev_seq;

    for(auto booking : m_wizard->booking_data.m_engine_data->prev_bookings){
        if (booking.schedule_id == break_id){
            prev_seq.push_back(booking.book_seq);
        }
    }

    auto gen_next_seq = [&](int rand){
        return (
            ( std::find(prev_seq.begin(), prev_seq.end(), rand) != prev_seq.end() ) ? true : false );
    };

    int next_seq{-1};

    std::random_device engine;
    std::uniform_int_distribution<int> distrib(1, max_spots);

    do{
        next_seq = distrib(engine);
    }while(gen_next_seq(next_seq));


    return next_seq;
}

} // namespace