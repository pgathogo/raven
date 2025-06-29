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

BreakLayoutForm::BreakLayoutForm(BreakLayout* bl,
                                 QDialog* parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::BreakLayoutForm)
    ,mBreakLayout{ bl }
    ,tempFromHr{0}
    ,tempToHr{0}
    ,m_line_no{0}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    mEDMBreakLine = std::make_unique<EntityDataModel>(
                std::make_shared<BreakLayoutLine>());

    ui->tvBreakLayoutLine->setModel(mEDMBreakLine.get());
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
    mRavenSetup = dynamic_cast<RavenSetup*>(mEdmTSetup->firstEntity().get());

    populateFormWidgets();

    connect(ui->cbTimeInterval, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::timeIntervalChanged);
    connect(ui->cbBreakFillMethod, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::break_fill_method_changed );

    connect(ui->btnCopy, &QPushButton::clicked, this, &BreakLayoutForm::copyHourClicked);
    connect(ui->btnUndo, &QPushButton::clicked, this, &BreakLayoutForm::undoCopyClicked);


    connect(ui->tbInsert, &QToolButton::clicked, this, &BreakLayoutForm::insert_row);
    connect(ui->tbDelete, &QToolButton::clicked, this, &BreakLayoutForm::delete_row);

    hideSaveNewBtn();

    set_defaults();

    setup_ui();
}

BreakLayoutForm::~BreakLayoutForm()
{
    delete ui;
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
   mBreakLayout->setName(ui->edtName->text().toStdString());
   auto set_bit = [](Qt::CheckState cs){ return (cs==Qt::Checked) ? 1 : 0; };
   mBreakLayout->setMonBit(set_bit(ui->cbMon->checkState()));
   mBreakLayout->setTueBit(set_bit(ui->cbTue->checkState()));
   mBreakLayout->setWedBit(set_bit(ui->cbWed->checkState()));
   mBreakLayout->setThuBit(set_bit(ui->cbThu->checkState()));
   mBreakLayout->setFriBit(set_bit(ui->cbFri->checkState()));
   mBreakLayout->setSatBit(set_bit(ui->cbSat->checkState()));
   mBreakLayout->setSunBit(set_bit(ui->cbSun->checkState()));

   mBreakLayout->setWeekDays(mBreakLayout->access_bit_tostring());
}

void BreakLayoutForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mBreakLayout->name()->value()));
    populate_choice_combo_int(ui->cbTimeInterval, mBreakLayout->timeInterval());

    auto cs = [](int i){ return (i==0) ? Qt::Unchecked : Qt::Checked; };
    ui->cbMon->setCheckState(cs(mBreakLayout->monBit()->value()));
    ui->cbTue->setCheckState(cs(mBreakLayout->tueBit()->value()));
    ui->cbWed->setCheckState(cs(mBreakLayout->wedBit()->value()));
    ui->cbThu->setCheckState(cs(mBreakLayout->thuBit()->value()));
    ui->cbFri->setCheckState(cs(mBreakLayout->friBit()->value()));
    ui->cbSat->setCheckState(cs(mBreakLayout->satBit()->value()));
    ui->cbSun->setCheckState(cs(mBreakLayout->sunBit()->value()));

    populateCopyCB();


    //populateBreakLine();

}

void BreakLayoutForm::clear_widgets()
{
    ui->edtName->clear();
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
    if (mBreakLayout->isNew()){
        clearBreaks();
        addBreakLines(0, mBreakLayout->timeInterval()->value());
    }else{
        BreakLayoutLine bll;
        auto fk_filter = std::make_tuple(
                    bll.breakLayout()->dbColumnName(),
                    "=",
                    mBreakLayout->id());
        std::string filter = mEDMBreakLine->prepareFilter(fk_filter);
        mEDMBreakLine->search(filter);
    }
}

void BreakLayoutForm::addBreakLines(int hour, int timeInterval)
{
    int time_interval;
    if (timeInterval == 0)
        time_interval = mRavenSetup->breakTimeInterval()->value();
    else
        time_interval = timeInterval;

    int breaks_per_hr = 60 / time_interval;
    int break_dur = mRavenSetup->breakDuration()->value();
    int max_spots = mRavenSetup->breakMaxSpots()->value();

    std::string fill_method = ui->cbBreakFillMethod->itemData(
                                                       ui->cbBreakFillMethod->currentIndex()).toString().toStdString();
    int min = 0;

    for (int ti=1; ti <= breaks_per_hr; ++ti){
        auto breakLine = std::make_unique<BreakLayoutLine>();
        QTime time(hour, min, 0);
        breakLine->setBreakTime(time);
        breakLine->setBreakHour(hour);
        breakLine->setDuration(break_dur);
        breakLine->setMaxSpots(max_spots);
        breakLine->setWeekDay(1);
        breakLine->set_break_fill_method(fill_method);
        breakLine->setRowId(++m_line_no);

        breakLine->setDBAction(DBAction::dbaCREATE);
        mEDMBreakLine->cacheEntity(std::move(breakLine));

        min += time_interval;
    }
}

std::vector<EntityRecord> const& BreakLayoutForm::breakLines() const
{
    return  mEDMBreakLine->modelEntities();
}

void BreakLayoutForm::clearBreakLineModel(int startHr, int endHr)
{
    while (startHr <= endHr){

        for (auto& [name, entity] : mEDMBreakLine->modelEntities()){
            BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());
            if (startHr == bll->breakHour()->value()){
                bll->setDBAction(DBAction::dbaDELETE);
            }
        }

        ++startHr;
    }

    mEDMBreakLine->deleteFromModel();
}

void BreakLayoutForm::copyHourClicked()
{
    startPos = mEDMBreakLine->count();

    copyHour(ui->cbFromHour->currentData().toInt(),
             ui->cbToHour->currentData().toInt() );

    endPos = mEDMBreakLine->count();
}

void BreakLayoutForm::undoCopyClicked()
{
    undoCopy(ui->cbFromHour->currentData().toInt(),
             ui->cbToHour->currentData().toInt());
}

void BreakLayoutForm::clearBreakTableView(int startRow, int endRow)
{
    for (int i=startRow; i<= endRow; ++i)
        ui->tvBreakLayoutLine->selectRow(i);

    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedRows();
    int countRows = indexes.count();

    for (int i=countRows; i>0; i--)
        mEDMBreakLine->removeRow(indexes.at(i-1).row(), QModelIndex());

}

void BreakLayoutForm::set_defaults()
{

    populate_choice_combo_string(ui->cbBreakFillMethod, mBreakLayout->break_fill_method());

    if (mBreakLayout->isNew()){
        ui->cbTimeInterval->setCurrentIndex(0);
        ui->cbBreakFillMethod->setCurrentIndex(0);
    }
    else{
        populateBreakLine();
    }
}

void BreakLayoutForm::copyHour(int fromHr, int toHr)
{
    if (toHr <= fromHr){
        showMessage("`To` hour less than `From` hour!");
        return;
    }

    for (; fromHr <= toHr; ++fromHr){
        addBreakLines(fromHr);
    }

}

void BreakLayoutForm::clearBreaks()
{
    clearBreakTableView(0, mEDMBreakLine->count());
    clearBreakLineModel(0,0);
}

void BreakLayoutForm::undoCopy(int fromHr, int toHr)
{

    clearBreakTableView(startPos, endPos);

    clearBreakLineModel(fromHr, toHr);

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

    populateBreakLine();
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

    auto entities = mEDMBreakLine->modelEntities();

    auto [unique_id, row_id] = row_identity();

    std::shared_ptr<BreakLayoutLine> tmp_bll = std::make_shared<BreakLayoutLine>();

    auto init_bll = [&](BreakLayoutLine* bll){
        tmp_bll->setBreakHour(bll->breakHour()->value());
        tmp_bll->setDuration(bll->duration()->value());
        tmp_bll->setBreakTime(bll->breakTime()->value());
        tmp_bll->setMaxSpots(bll->maxSpots()->value());
        tmp_bll->set_break_fill_method(bll->break_fill_method()->value());
        tmp_bll->setDBAction(DBAction::dbaCREATE);
    };

    for (auto& [entity_name, entity]: entities)
    {
        BreakLayoutLine* bll = dynamic_cast<BreakLayoutLine*>(entity.get());

        if (unique_id > -1){
            if (bll->id() == unique_id){
                tmp_bll->setId(unique_id);
                init_bll(bll);
                break;
            }
        }else{
            if (bll->rowId()->value() == row_id) {
                tmp_bll->setRowId(++m_line_no);
                init_bll(bll);
                break;
            }
        }
    }

    mEDMBreakLine->cacheEntity(std::move(tmp_bll));
}

void BreakLayoutForm::delete_row()
{
    QModelIndexList indexes = ui->tvBreakLayoutLine->selectionModel()->selectedRows();
    if (indexes.count() == 0)
        return;

    auto [unique_id, line_no] = row_identity();

    if (unique_id > -1 ){
        EntityDataModel edm(std::make_shared<BreakLayoutLine>());
        edm.deleteEntityByValue({"id", unique_id});
    }

    int row = indexes[0].row();

    ui->tvBreakLayoutLine->model()->removeRow(row);

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
