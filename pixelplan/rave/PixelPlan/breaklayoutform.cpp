#include <print>

#include "breaklayoutform.h"
#include "ui_breaklayoutform.h"

#include "../../../rave/framework/ui_baseentitydetaildlg.h"
#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/entitydatamodel.h"
#include "../../../rave/framework/ravensetup.h"
#include "../../../rave/framework/comboboxitemdelegate.h"
#include "../../../rave/framework/readonlydelegate.h"
#include "../../../rave/framework/spinboxdelegate.h"
#include "../../../rave/framework/timeeditdelegate.h"

#include "../../../rave/utils/notificationbar.h"

#include "breaklayout.h"
#include "breaklayoutline.h"
#include "tvprogram.h"

BreakLayoutForm::BreakLayoutForm(BreakLayout* bl, std::vector<int> excl_progids,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::BreakLayoutForm)
    ,mBreakLayout{ bl }
    ,tempFromHr{0}
    ,tempToHr{0}
    ,m_line_no{0}
    ,m_edm_tvprogram{nullptr}
    ,m_current_tvprogram{nullptr}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    m_edm_breakline = std::make_unique<EntityDataModel>(
                std::make_shared<BreakLayoutLine>());

    ui->tvBreakLayoutLine->setModel(m_edm_breakline.get());
    ui->tvBreakLayoutLine->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    TimeEditDelegate* ted = new TimeEditDelegate(ui->tvBreakLayoutLine);
    ui->tvBreakLayoutLine->setItemDelegateForColumn(0, ted);

    SpinBoxDelegate* sbd = new SpinBoxDelegate(ui->tvBreakLayoutLine);
    ui->tvBreakLayoutLine->setItemDelegateForColumn(1, sbd);
    ui->tvBreakLayoutLine->setItemDelegateForColumn(2, sbd);

    std::map<QString, QVariant> data;
    data["Sequence"] = "S";
    data["Random"] = "R";
    ComboBoxItemDelegate*  cbid = new ComboBoxItemDelegate(data, ui->tvBreakLayoutLine);
    ui->tvBreakLayoutLine->setItemDelegateForColumn(3, cbid);

    ReadOnlyDelegate* rod = new ReadOnlyDelegate(ui->tvBreakLayoutLine);
    ui->tvBreakLayoutLine->setItemDelegateForColumn(4, rod);

    mEdmTSetup = std::make_unique<EntityDataModel>(
                std::make_shared<RavenSetup>());
    mEdmTSetup->all();
    if (mEdmTSetup->count() > 0)
        mRavenSetup = dynamic_cast<RavenSetup*>(mEdmTSetup->firstEntity().get());

    populateFormWidgets();

    connect(ui->cbTimeInterval, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::timeIntervalChanged);

    connect(ui->cbProgram, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::on_tvprogram_changed);

    connect(ui->cbBreakFillMethod, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::break_fill_method_changed );

    connect(ui->btnCopy, &QPushButton::clicked, this, &BreakLayoutForm::copyHourClicked);
    connect(ui->btnUndo, &QPushButton::clicked, this, &BreakLayoutForm::undoCopyClicked);


    connect(ui->tbInsert, &QToolButton::clicked, this, &BreakLayoutForm::insert_row);
    connect(ui->tbDelete, &QToolButton::clicked, this, &BreakLayoutForm::delete_row);


    std::println("AAA");

    populate_program_combo(excl_progids);

    std::println("BBB");

    if (!mBreakLayout->isNew()) {
        int index = ui->cbProgram->findData(QVariant(mBreakLayout->tvprogram()->value()));
        ui->cbProgram->setCurrentIndex(index);
        ui->cbProgram->setEnabled(false);
    }


    ui->gbCopy->setHidden(true);
    hideSaveNewBtn();

    set_defaults();

    std::println("CCC");

    setup_ui();

    std::println("DDD");


}

BreakLayoutForm::~BreakLayoutForm()
{
    delete ui;
}

void BreakLayoutForm::populate_program_combo(std::vector<int> ex_progids)
{
    m_edm_tvprogram = std::make_unique<EntityDataModel>(std::make_unique<PIXELPLAN::TVProgram>());
    m_edm_tvprogram->all();

    if (m_edm_tvprogram->count() == 0)
        return;

    auto provider = m_edm_tvprogram->getDBManager()->provider();
    if(provider->cacheSize() == 0)
        return;

    provider->cache()->first();

    do {
        auto it_begin = provider->cache()->currentElement()->begin();
        auto it_end = provider->cache()->currentElement()->end();

        int id = -1;
        QString title;

        for (; it_begin != it_end; ++it_begin) {
            std::string field_name = (*it_begin).first;
            std::string field_value = (*it_begin).second;

            if (field_name == "id")
                id  = std::stoi(field_value);
            if (field_name == "title")
                title = QString::fromStdString(field_value);
        }

        // Only show TVPrograms not in the exclusion list
        if (std::find(ex_progids.begin(), ex_progids.end(), id) == ex_progids.end())
            ui->cbProgram->addItem(title, QVariant(id));


        provider->cache()->next();
    }  while (!provider->cache()->isLast());

}

void BreakLayoutForm::setup_ui()
{
    QPixmap ins_row(":/images/media/icons/insert_row.png");
    QIcon ins_icon(ins_row);
    ui->tbInsert->setIcon(ins_icon);

    QPixmap del_row(":/images/media/icons/remove_row.png");
    QIcon del_icon(del_row);
    ui->tbDelete->setIcon(del_icon);
}

ActionResult BreakLayoutForm::saveRecord()
{
    populateEntityFields();
    return mBreakLayout->validate();
}

std::string BreakLayoutForm::windowTitle()
{
    return "Create Break Layout";
}

void BreakLayoutForm::populateEntityFields()
{
   // mBreakLayout->setName(ui->edtName->text().toStdString());
   auto set_bit = [](Qt::CheckState cs){ return (cs==Qt::Checked) ? 1 : 0; };
   mBreakLayout->setMonBit(set_bit(ui->cbMon->checkState()));
   mBreakLayout->setTueBit(set_bit(ui->cbTue->checkState()));
   mBreakLayout->setWedBit(set_bit(ui->cbWed->checkState()));
   mBreakLayout->setThuBit(set_bit(ui->cbThu->checkState()));
   mBreakLayout->setFriBit(set_bit(ui->cbFri->checkState()));
   mBreakLayout->setSatBit(set_bit(ui->cbSat->checkState()));
   mBreakLayout->setSunBit(set_bit(ui->cbSun->checkState()));

   mBreakLayout->setWeekDays(mBreakLayout->access_bit_tostring());

   int curr_index = ui->cbProgram->currentIndex();
   int tvprog_id = ui->cbProgram->itemData(curr_index).toInt();
   mBreakLayout->set_tvprogram(tvprog_id);

}

void BreakLayoutForm::populateFormWidgets()
{
    // ui->edtName->setText(stoq(mBreakLayout->name()->value()));
    populate_choice_combo_int(ui->cbTimeInterval, mBreakLayout->timeInterval());

    auto cs = [](int i){ return (i==0) ? Qt::Unchecked : Qt::Checked; };
    ui->cbMon->setCheckState(cs(mBreakLayout->monBit()->value()));
    ui->cbTue->setCheckState(cs(mBreakLayout->tueBit()->value()));
    ui->cbWed->setCheckState(cs(mBreakLayout->wedBit()->value()));
    ui->cbThu->setCheckState(cs(mBreakLayout->thuBit()->value()));
    ui->cbFri->setCheckState(cs(mBreakLayout->friBit()->value()));
    ui->cbSat->setCheckState(cs(mBreakLayout->satBit()->value()));
    ui->cbSun->setCheckState(cs(mBreakLayout->sunBit()->value()));

    //populateCopyCB();


    //populateBreakLine();

}

void BreakLayoutForm::clear_widgets()
{
    // ui->edtName->clear();
}

void BreakLayoutForm::populate_choice_combo_int(QComboBox* cbox, const ChoiceField<int>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), std::get<0>(c));

    cbox->setCurrentIndex( cbox->findData(QVariant(cf->value())) );
}

void BreakLayoutForm::populate_choice_combo_string(QComboBox* cbox, const ChoiceField<std::string>* cf)
{
    for (const auto& [value, title] : cf->choices())
        cbox->addItem(stoq(title), stoq(value));

    cbox->setCurrentIndex( cbox->findData(QVariant(QString::fromStdString(cf->value()))) );
}

void BreakLayoutForm::populateCopyCB()
{
    for(int hr=0; hr<24; ++hr){
        ui->cbFromHour->addItem(stoq(std::to_string(hr)), hr);
        ui->cbToHour->addItem(stoq(std::to_string(hr)), hr);
    }
    ui->cbFromHour->setCurrentIndex(1);
}

void BreakLayoutForm::populateBreakLine()
{
    clearBreaks();

    int curr_index = ui->cbTimeInterval->currentIndex();

    if (curr_index == -1)
        return;

    int interval =  ui->cbTimeInterval->itemData(curr_index).toInt();

    std::println("666");


    add_break_lines(m_current_tvprogram, interval);

    std::println("777");

}

void BreakLayoutForm::show_breaklines_from_db(int break_layout_id)
{
    BreakLayoutLine bll;

    auto fk_filter = std::make_tuple(
            bll.breakLayout()->dbColumnName(),
            "=",
            break_layout_id);

    std::string filter = m_edm_breakline->prepareFilter(fk_filter);

    m_edm_breakline->search(filter);

}

void BreakLayoutForm::add_break_lines(std::shared_ptr<PIXELPLAN::TVProgram> tvprogram, int time_interval)
{
    if (tvprogram == nullptr)
        return;

    int break_time_interval = 0;
    int break_duration      = 0;
    int break_max_spots     = 0;

    if (mEdmTSetup->count() > 0) {

        break_time_interval =  mRavenSetup->breakTimeInterval()->value();
        break_duration = mRavenSetup->breakDuration()->value();
        break_max_spots = mRavenSetup->breakMaxSpots()->value();
    }

    std::string fill_method = ui->cbBreakFillMethod->itemData(
                                 ui->cbBreakFillMethod->currentIndex()).toString().toStdString();


    int hour = tvprogram->start_time()->value().hour();
    int min  = tvprogram->start_time()->value().minute();

    QTime start_time(hour, min, 0);

    QTime t2 = start_time;

    do {
        auto breakLine = std::make_unique<BreakLayoutLine>();

        t2 = t2.addSecs(time_interval * 60);

        breakLine->setBreakTime(t2);
        breakLine->setBreakHour(hour);
        breakLine->setDuration(break_duration);
        breakLine->setMaxSpots(break_max_spots);
        breakLine->setWeekDay(1);
        breakLine->set_break_fill_method(fill_method);
        breakLine->setRowId(++m_line_no);

        breakLine->setDBAction(DBAction::dbaCREATE);
        m_edm_breakline->cacheEntity(std::move(breakLine));

    } while(t2 < tvprogram->end_time()->value() );
}


std::vector<EntityRecord> const& BreakLayoutForm::breakLines() const
{
    return  m_edm_breakline->modelEntities();
}

void BreakLayoutForm::clearBreakLineModel(int startHr, int endHr)
{
    while (startHr <= endHr){

        for (auto& [name, entity] : m_edm_breakline->modelEntities()){
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());
            if (startHr == bll->breakHour()->value()){
                bll->setDBAction(DBAction::dbaDELETE);
            }
        }

        ++startHr;
    }

    m_edm_breakline->deleteFromModel();
}

void BreakLayoutForm::copyHourClicked()
{
    startPos = m_edm_breakline->count();

    copyHour(ui->cbFromHour->currentData().toInt(),
             ui->cbToHour->currentData().toInt() );

    endPos = m_edm_breakline->count();
}

void BreakLayoutForm::undoCopyClicked()
{
    undoCopy(ui->cbFromHour->currentData().toInt(),
             ui->cbToHour->currentData().toInt());
}

void BreakLayoutForm::clearBreakTableView(int startRow, int endRow)
{
    m_edm_breakline->clearEntities();

}

void BreakLayoutForm::set_defaults()
{

    populate_choice_combo_string(ui->cbBreakFillMethod, mBreakLayout->break_fill_method());

    if (mBreakLayout->isNew()){

        std::println("111");

         ui->cbTimeInterval->setCurrentIndex(0);
         ui->cbBreakFillMethod->setCurrentIndex(0);

         //populateBreakLine();

    }
    else{
        show_breaklines_from_db(mBreakLayout->id());
    }
}

void BreakLayoutForm::copyHour(int fromHr, int toHr)
{
    if (toHr <= fromHr){
        showMessage("`To` hour less than `From` hour!");
        return;
    }

    for (; fromHr <= toHr; ++fromHr){
        //addBreakLines(fromHr);
    }

}

void BreakLayoutForm::clearBreaks()
{
    m_edm_breakline->clearEntities();
    //clearBreakTableView(0, mEDMBreakLine->count());

    clearBreakLineModel(0,0);
}

void BreakLayoutForm::undoCopy(int fromHr, int toHr)
{

    clearBreakTableView(startPos, endPos);

    //clearBreakLineModel(fromHr, toHr);

    /*
    for (int i=startPos; i<= endPos; ++i)
        ui->tvBreakLayoutLine->selectRow(i);

    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedRows();
    int countRows = indexes.count();

    for (int i=countRows; i>0; i--)
        mEDMBreakLine->removeRow(indexes.at(i-1).row(), QModelIndex());


    while (from <= to){

        for (auto& [name, entity] : mEDMBreakLine->modelEntities()){
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());
            if (from == bll->breakHour()->value()){
                bll->setDBAction(DBAction::dbaDELETE);
            }
        }

        ++from;
    }

    mEDMBreakLine->deleteFromModel();
    */

}

void BreakLayoutForm::timeIntervalChanged(int i)
{
    mBreakLayout->setTimeInterval(
                ui->cbTimeInterval->itemData(i).toInt());

    std::println("222");

    populateBreakLine();


    // if (mBreakLayout->isNew())
    //     populateBreakLine();
}


void BreakLayoutForm::on_tvprogram_changed(int i)
{
    if (m_edm_tvprogram == nullptr)
        return;

    if (m_edm_tvprogram->count() == 0)
        return;

    if (ui->cbProgram->count() == 0)
        return;

    auto base_entity =  m_edm_tvprogram->find_entity_by_id(
        ui->cbProgram->itemData(i).toInt() );

    m_current_tvprogram = std::dynamic_pointer_cast<PIXELPLAN::TVProgram>(base_entity);

    populateBreakLine();

    ui->lblStartTime->setText("Start Time: "+m_current_tvprogram->start_time()->value().toString("hh:mm"));
    ui->lblEndTime->setText("End Time: "+m_current_tvprogram->end_time()->value().toString("hh:mm"));
}

void BreakLayoutForm::break_fill_method_changed(int index)
{
    mBreakLayout->set_break_fill_method(
        ui->cbBreakFillMethod->itemData(index).toString().toStdString());

    set_break_fill_method( ui->cbBreakFillMethod->currentText() );
}

void BreakLayoutForm::set_break_fill_method(QString text)
{
    const int COL_BREAK_FILL_METHOD = 3;

    auto model = ui->tvBreakLayoutLine->model();

    for(int row=0; row < ui->tvBreakLayoutLine->model()->rowCount(); ++row)
    {
        for(int col=0; col< ui->tvBreakLayoutLine->model()->columnCount(); ++col)
        {
            if (col == COL_BREAK_FILL_METHOD)
            {
                auto index = model->index(row, col);
                model->setData(index, text);
            }
        }

    }
}

QAbstractItemModel* BreakLayoutForm::breakline_model()
{
    return ui->tvBreakLayoutLine->model();
    // return m_edm_breakline;
}


std::tuple<int, int> BreakLayoutForm::row_identity()
{
    int unique_id = -1;
    int row_id = -1;
    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedIndexes();

    for (int i=0; i<indexes.size(); ++i){
        QVariant data = indexes[i].data(Qt::EditRole);
        switch (i)
        {
        case 4:
            unique_id = data.toInt();
            break;
        case 5:
            row_id = data.toInt();
            break;
        }
    }

    return std::make_tuple(unique_id, row_id);

}

void BreakLayoutForm::insert_row()
{
    std::println("Inserting a row...");

    std::shared_ptr<BreakLayoutLine> tmp_bll = std::make_shared<BreakLayoutLine>();

    auto init_bll = [&](std::shared_ptr<BreakLayoutLine> bll){
        tmp_bll->setBreakHour(bll->breakHour()->value());
        tmp_bll->setDuration(bll->duration()->value());

        int curr_index = ui->cbTimeInterval->currentIndex();
        int interval = ui->cbTimeInterval->itemData(curr_index).toInt();

        tmp_bll->setBreakTime(bll->breakTime()->value().addSecs(interval*60));

        tmp_bll->setMaxSpots(bll->maxSpots()->value());
        tmp_bll->set_break_fill_method(bll->break_fill_method()->value());
        tmp_bll->setRowId(++m_line_no);
        tmp_bll->setDBAction(DBAction::dbaCREATE);
    };


    auto be = m_edm_breakline->get_entity_at_row(m_edm_breakline->count()-1);

    if (be == nullptr)
        return;

    std::shared_ptr<BreakLayoutLine> bll = std::dynamic_pointer_cast<BreakLayoutLine>(be);

    init_bll(bll);


    m_edm_breakline->cacheEntity(std::move(tmp_bll));
}

void BreakLayoutForm::delete_row()
{
    QModelIndexList rows = ui->tvBreakLayoutLine->selectionModel()->selectedRows();
    if (rows.count() == 0)
        return;

    // auto [unique_id, line_no] = row_identity();
    // if (unique_id > -1 ){
    //     EntityDataModel edm(std::make_shared<BreakLayoutLine>());
    //     edm.deleteEntityByValue({"id", unique_id});
    // }

    int row = rows[0].row();

    auto be = m_edm_breakline->get_entity_at_row(row);
    if (be == nullptr)
        return;

    std::shared_ptr<BreakLayoutLine> bll = std::dynamic_pointer_cast<BreakLayoutLine>(be);
    m_edm_breakline->deleteEntity(*(bll.get()));

    ui->tvBreakLayoutLine->model()->removeRow(row);

}

void BreakLayoutForm::delete_row_TEST()
{

    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedIndexes();
    if (indexes.count() == 0)
        return;

    int unique_id = -1;
    int row_id = -1;

    for(int i=0; i < indexes.count(); ++i) {
        QVariant data = indexes[i].data(Qt::EditRole);
        switch (i)
        {
        case 4:
            unique_id = data.toInt();
            break;
        case 5:
            row_id = data.toInt();
            break;
        }

    }

    qDebug() << "Unique ID: "<< unique_id;
    qDebug() << "Row ID: "<< row_id;

    // if (unique_id > -1) {
    //     EntityDataModel edm(std::make_shared<BreakLayoutLine>());
    //     edm.deleteEntityByValue({"id", unique_id});
    // }

}


void BreakLayoutForm::test_model()
{
    auto& break_lines = breakLines();

    for(auto& break_line : break_lines){
        BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(std::get<1>(break_line).get());
    }
}


QModelIndexList BreakLayoutForm::selected_indexes()
{
    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedRows();
    return indexes;
}
