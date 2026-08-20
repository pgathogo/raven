#include <chrono>
#include <algorithm>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include <QSpacerItem>
#include <QGroupBox>
#include <QCheckBox>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>

#include "../../../rave/utils/tools.h"
#include "../../../rave/framework/entitydatamodel.h"

#include <spot.h>
#include "bookingdatepage.h"
#include "tvprogram.h"
#include "orderbookingwizard.h"

namespace PIXELPLAN
{

    BookingDatePage::BookingDatePage(OrderBookingWizard* wiz)
        :m_wizard(wiz)
    {
        setTitle("Booking Date");
        setSubTitle("Select range of dates to booking order");

        setup_ui();

        get_tv_programs(m_programs);

        show_tv_programs(m_programs);
    }

    void BookingDatePage::setup_ui()
    {
        QVBoxLayout* main_layout = new QVBoxLayout();

        // --------- Date Section  ------------------
        QHBoxLayout* date_layout = new QHBoxLayout();

        QLabel* lbl_start_date = new QLabel("Start Date:");
        m_de_start_date = new QDateEdit();
        m_de_start_date->setCalendarPopup(true);

        QLabel* lbl_end_date = new QLabel("End Date:");
        m_de_end_date = new QDateEdit();
        m_de_end_date->setCalendarPopup(true);

        date_layout->addWidget(lbl_start_date);
        date_layout->addWidget(m_de_start_date, 1);

        date_layout->addSpacing(40);

        date_layout->addWidget(lbl_end_date);
        date_layout->addWidget(m_de_end_date, 1);

        date_layout->addSpacing(40);

        main_layout->addLayout(date_layout);



        // --------- TV Programs Section  ------------------
        QHBoxLayout* hl_tv_breaks_layout = new QHBoxLayout();

        QVBoxLayout* program_layout = new QVBoxLayout();
        QGroupBox* grp_tvprog = new QGroupBox("TV Programs: ");

        m_cb_all_programs = new QCheckBox("All Programs");
        connect(m_cb_all_programs, &QCheckBox::toggled, this, &BookingDatePage::select_all_programs);

        m_lw_programs = new QListWidget();
        m_lw_programs->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_lw_programs->setAlternatingRowColors(true);
        connect(m_lw_programs, &QListWidget::itemClicked, this, &BookingDatePage::on_program_clicked);

        program_layout->addWidget(m_cb_all_programs);
        program_layout->addWidget(m_lw_programs);

        grp_tvprog->setLayout(program_layout);

        hl_tv_breaks_layout->addWidget(grp_tvprog, 1);

        // --------- Breaks Section  ------------------

        QVBoxLayout* vl_breaks_layout = new QVBoxLayout();
        QGroupBox* grp_breaks = new QGroupBox("Breaks:");

        QHBoxLayout* hl_sel_breaks = new QHBoxLayout();

        m_cb_all_breaks = new QCheckBox("All Breaks");
        connect(m_cb_all_breaks, &QCheckBox::toggled, this, &BookingDatePage::select_all_breaks);

        QLabel* lbl_sel_breaks = new QLabel("Selected Breaks:");
        m_lbl_selected_break_count = new QLabel("0");

        hl_sel_breaks->addWidget(m_cb_all_breaks);
        hl_sel_breaks->addSpacing(100);
        hl_sel_breaks->addWidget(lbl_sel_breaks, 0);
        hl_sel_breaks->addWidget(m_lbl_selected_break_count, 1);

        m_tw_breaks = new QTableWidget();
        connect(m_tw_breaks, &QTableWidget::itemChanged, this, &BookingDatePage::on_break_sel_changed);
        set_breaks_table(m_tw_breaks);

        vl_breaks_layout->addLayout(hl_sel_breaks);
        vl_breaks_layout->addWidget(m_tw_breaks);

        grp_breaks->setLayout(vl_breaks_layout);

        hl_tv_breaks_layout->addWidget(grp_breaks, 2);

        main_layout->addLayout(hl_tv_breaks_layout);


        setLayout(main_layout);

    }

    void BookingDatePage::set_breaks_table(QTableWidget* tw)
    {
        QStringList header;
        header << "" << "Break Time" << "Duration" << "Max Spots" << "Fill Pos";

        tw->setColumnCount(5);
        tw->setHorizontalHeaderLabels(header);

        //ui->twBreaks->setRowCount(row_count);

        tw->setColumnWidth(0, 20);
    }

    void BookingDatePage::on_program_clicked(QListWidgetItem* item)
    {
        QItemSelectionModel* ism = m_lw_programs->selectionModel();
        QModelIndexList indexes = ism->selectedIndexes();

        if (indexes.size() == 0)
            return;

        std::vector<ProgramBreak> breaks;

        int prog_id = 0;

        for(auto& index : indexes)
        {
           prog_id = index.data(Qt::UserRole).toInt();

           for(auto brk: m_programs[prog_id].breaks)  {
               breaks.push_back(brk);
           }

        }

        show_program_breaks(breaks);

        // if (prog_id > 0)
        //     show_program_breaks(m_programs[prog_id].breaks);
    }

    void BookingDatePage::on_break_sel_changed(QTableWidgetItem* item)
    {
        if (item->column() == 0) {

        m_tw_breaks->blockSignals(true);

        if (item->checkState() == Qt::Checked) {
            m_tw_breaks->selectRow(item->row());
            ++m_selected_breaks_count;

        } else {

            if (item->checkState() == Qt::Unchecked) {
                QModelIndex index = m_tw_breaks->model()->index(item->row(), item->column());
                QItemSelection selection;
                selection.select(index, index);
                m_tw_breaks->selectionModel()->select(selection, QItemSelectionModel::Deselect | QItemSelectionModel::Rows);

            if (m_selected_breaks_count > 0)
                --m_selected_breaks_count;
            }
        }

            m_tw_breaks->blockSignals(false);
        }

        QString count = QString("%1").arg(QString::number(m_selected_breaks_count));
        m_lbl_selected_break_count->setText(count);

    }

    void BookingDatePage::select_all_breaks(bool checked)
    {
        for (int row=0; row < m_tw_breaks->rowCount(); row++) {
            auto item = m_tw_breaks->item(row, 0);
            if (item) {
                item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
                item->setSelected(checked);
            }
        }

        m_wizard->booking_data.is_all_breaks = checked;

    }


    void BookingDatePage::show_program_breaks(std::vector<ProgramBreak>& breaks)
    {
        if (breaks.size() == 0)
            return;

        std::sort(breaks.begin(), breaks.end(), [](const ProgramBreak& lhs, ProgramBreak& rhs) {
            return (lhs.break_time < rhs.break_time);
        });

        m_tw_breaks->clearContents();
        m_tw_breaks->setRowCount(0);

        constexpr int CHECK_COL = 0;
        constexpr int BREAK_TIME_COL = 1;
        constexpr int DURATION_COL   = 2;
        constexpr int MAX_SPOTS_COL  = 3;
        constexpr int FILL_POS_COL   = 4;

        QStringList fill_pos = {"First", "In-Between", "Last"};

        int row = m_tw_breaks->rowCount();

        for (auto& pb: breaks) {

            m_tw_breaks->insertRow(row);

            // Checkbox
            QTableWidgetItem* cbox_item = new QTableWidgetItem();
            cbox_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            cbox_item->setCheckState(Qt::Unchecked);
            m_tw_breaks->setItem(row, CHECK_COL, cbox_item);

            // Break Time
            auto twi_btime = new QTableWidgetItem(pb.break_time);
            twi_btime->setData(Qt::UserRole, pb.line_id);
            m_tw_breaks->setItem(row, BREAK_TIME_COL, twi_btime);

            // Duration
            auto twi_duration = new QTableWidgetItem(QString::number(pb.duration));
            m_tw_breaks->setItem(row, DURATION_COL, twi_duration);

            // Max spots
            auto twi_max_spots = new QTableWidgetItem(QString::number(pb.max_spots));
            m_tw_breaks->setItem(row, MAX_SPOTS_COL, twi_max_spots);

            // Fill position combobox
            QComboBox* cb = new QComboBox();
            cb->addItems(fill_pos);
            m_tw_breaks->setCellWidget(row, FILL_POS_COL, cb);

            ++row;

        }

    }

    bool BookingDatePage::validatePage()
    {
        if (m_de_end_date->date() < m_de_start_date->date()) {
            showMessage("End date less than start date", QMessageBox::Critical);
            return false;
        }

        m_wizard->booking_data.start_date = m_de_start_date->date();
        m_wizard->booking_data.end_date = m_de_end_date->date();

        return true;

    }


    void BookingDatePage::get_tv_programs(std::map<progid, Program>& programs)
    {
        //auto start = std::chrono::steady_clock::now();

        std::stringstream sql;
        sql << "SELECT rave_tvprogram.id AS progid, rave_tvprogram.title, "
            <<  " rave_breaklayoutline.id AS line_id, rave_breaklayoutline.break_time, "
            <<  " rave_breaklayoutline.break_hour,  rave_breaklayoutline.duration, "
            <<  " rave_breaklayoutline.max_spots, rave_breaklayoutline.break_fill_method, "
            <<  " rave_breaklayout.week_days, rave_breaklayout.tvprogram_id "
        << " FROM rave_tvprogram, rave_breaklayout, rave_breaklayoutline "
        << "WHERE rave_tvprogram.id = rave_breaklayout.tvprogram_id "
        << "  AND rave_breaklayout.id = rave_breaklayoutline.break_layout_id "
        << "  AND rave_tvprogram.deleted = 0 "
        << " ORDER BY rave_breaklayout.tvprogram_id,  rave_breaklayoutline.break_time ";

        //auto edm = std::make_unique<EntityDataModel>(std::make_unique<PIXELPLAN::TVProgram>());

        //std::string where_clause = std::format(" WHERE deleted = 0 ");

        // edm->all();
        //edm->search_with_filter(where_clause);
        //if (edm->count() == 0)
        //return;

        EntityDataModel edm;
        edm.readRaw(sql.str());

        auto provider = edm.getDBManager()->provider();

        if (provider->cacheSize() == 0)
        return;

        provider->cache()->first();

        do {
            auto it_begin = provider->cache()->currentElement()->begin();
            auto it_end = provider->cache()->currentElement()->end();

            int prog_id = -1;
            QString title;

            ProgramBreak pb;

            for(; it_begin != it_end; ++it_begin) {

                std::string field_name = (*it_begin).first;
                std::string field_value = (*it_begin).second;

                if (field_name == "progid")
                    prog_id = std::stoi(field_value);

                if (field_name == "title")
                    title = QString::fromStdString(field_value);

                if (field_name == "line_id")
                    pb.line_id = to_int(field_value);

                if (field_name == "break_time")
                    pb.break_time = QString::fromStdString(field_value);

                if (field_name == "duration")
                    pb.duration = to_int(field_value);

                if (field_name == "max_spots")
                    pb.max_spots = to_int(field_value);

                if (field_name == "tvprogram_id")
                    pb.progid = to_int(field_value);
            }

            if (!programs.contains(prog_id)) {
                Program prg = {title};
                programs[prog_id] = prg;
            }

            programs[prog_id].breaks.push_back(pb);


            provider->cache()->next();

        } while(!provider->cache()->isLast());

        //auto end = std::chrono::steady_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        //std::cout << "Execution Time: " << duration.count() << " milliseconds\n";


    }

    void BookingDatePage::show_tv_programs(const std::map<progid, Program>& programs)
    {
        for (const auto& [id, prg] : programs)
        {
            auto lwi = new QListWidgetItem(prg.title);
            lwi->setData(Qt::UserRole, id);

            // If program has no breaks, show it in red
            if (prg.breaks.size() == 0) {
                lwi->setForeground(QBrush(QColor(Qt::red)));
            }

            m_lw_programs->addItem(lwi);

        }
    }

    void BookingDatePage::select_all_programs()
    {
        if (m_cb_all_programs->isChecked()) {

            m_lw_programs->selectAll();
            auto selected_items = m_lw_programs->selectedItems();
            for(int i=0; i < selected_items.count(); i++)  {
                m_lw_programs->itemClicked(selected_items.at(i));
            }
        } else {
            m_lw_programs->clearSelection();
            m_tw_breaks->clearContents();
            m_tw_breaks->setRowCount(0);
        }

    }

    bool BookingDatePage::is_all_breaks()
    {
        return m_cb_all_breaks->isChecked();

    }

    void BookingDatePage::fetch_selected_program_breaks()
    {
        //std::vector<SelectedProgramBreak> selected_breaks{};

        QModelIndexList mil = m_tw_breaks->selectionModel()->selectedRows();

        if (mil.size() == 0)
            return;

        constexpr int BREAK_TIME_COL = 1;
        constexpr int DURATION_COL   = 2;
        constexpr int MAX_SPOTS_COL  = 3;
        constexpr int FILL_POS       = 4;

        for (auto& index : mil)
        {
            int row = index.row();

            SelectedProgramBreak spb;

            QTableWidgetItem* twi_break_time = m_tw_breaks->item(row, BREAK_TIME_COL);
            spb.break_time = twi_break_time->text();

            QTableWidgetItem* twi_duration = m_tw_breaks->item(row, DURATION_COL);
            spb.duration =  twi_duration->text().toInt();

            QTableWidgetItem* twi_max_spots = m_tw_breaks->item(row, MAX_SPOTS_COL);
            spb.max_spots = twi_max_spots->text().toInt();

            QComboBox* combo = (QComboBox*)m_tw_breaks->cellWidget(row, FILL_POS);

            spb.fill_pos  = static_cast<FillPos>(combo->currentIndex());

            m_wizard->booking_data.selected_breaks.push_back(spb);

        }

        // return selected_breaks;
    }


} // Namespace

