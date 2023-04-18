#include "breaklayoutform.h"
#include "ui_breaklayoutform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../framework/choicefield.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravensetup.h"
#include "../utils/notificationbar.h"

#include "breaklayout.h"
#include "breaklayoutline.h"

BreakLayoutForm::BreakLayoutForm(BreakLayout* bl,
                                 QDialog* parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::BreakLayoutForm),
    mBreakLayout{ bl },
    tempFromHr{0},
    tempToHr{0}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    mEDMBreakLine = std::make_unique<EntityDataModel>(
                std::make_shared<BreakLayoutLine>());

    ui->tvBreakLayoutLine->setModel(mEDMBreakLine.get());
    ui->tvBreakLayoutLine->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    mEdmTSetup = std::make_unique<EntityDataModel>(
                std::make_shared<RavenSetup>());
    mEdmTSetup->all();
    mRavenSetup = dynamic_cast<RavenSetup*>(mEdmTSetup->firstEntity().get());

    populateFormWidgets();

    connect(ui->cbTimeInterval, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BreakLayoutForm::timeIntervalChanged);
    connect(ui->btnCopy, &QPushButton::clicked, this, &BreakLayoutForm::copyHourClicked);
    connect(ui->btnUndo, &QPushButton::clicked, this, &BreakLayoutForm::undoCopyClicked);

    hideSaveNewBtn();

    setDefaults();
}

BreakLayoutForm::~BreakLayoutForm()
{
    delete ui;
}

ActionResult BreakLayoutForm::saveRecord()
{
    populateEntityFields();
    return mBreakLayout->validate();
}

std::string BreakLayoutForm::windowTitle()
{
    return "Break Layout";
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
    populateChoiceCombo(ui->cbTimeInterval, mBreakLayout->timeInterval());

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

void BreakLayoutForm::populateChoiceCombo(QComboBox* cbox, const ChoiceField<int>* cf)
{
    for (const auto& c : cf->choices())
        cbox->addItem(stoq(std::get<1>(c)), std::get<0>(c));

    cbox->setCurrentIndex( cbox->findData(QVariant(cf->value())) ); }

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

        int min = 0;

        for (int ti=1; ti <= breaks_per_hr; ++ti){
            auto breakLine = std::make_unique<BreakLayoutLine>();
            QTime time(hour, min, 0);
            breakLine->setBreakTime(time);
            breakLine->setBreakHour(hour);
            breakLine->setDuration(break_dur);
            breakLine->setMaxSpots(max_spots);
            breakLine->setWeekDay(1);
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

    qDebug() << "Selected Rows: " << countRows;

    for (int i=countRows; i>0; i--)
        mEDMBreakLine->removeRow(indexes.at(i-1).row(), QModelIndex());

}

void BreakLayoutForm::setDefaults()
{
    if (mBreakLayout->isNew())
        ui->cbTimeInterval->setCurrentIndex(0);
    else
        populateBreakLine();
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
