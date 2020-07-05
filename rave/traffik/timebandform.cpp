#include <QDebug>
#include "timebandform.h"
#include "ui_timebandform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../utils/tools.h"
#include "../framework/choicefield.h"

#include "timeband.h"

TimeBandForm::TimeBandForm(TimeBand* tband,
                           QDialog* parent) :
    BaseEntityDetailDlg{parent},
    ui(new Ui::TimeBandForm),
    mTimeBand{tband},
    mDPG{}

{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    ui->tabWidget->setCurrentIndex(0);
    mDPG = new DayPartGrid(ui->vlDaypart);

    connect(ui->rbIncPercent, &QRadioButton::toggled, this, &TimeBandForm::onIncPercent);

    populateFormWidgets();
    populateGrid();
}

TimeBandForm::~TimeBandForm()
{
    delete mDPG;
    delete ui;
}

ActionResult TimeBandForm::saveRecord()
{
    populateEntityFields();
    ActionResult ar = mTimeBand->validate();
    return ar;
}

std::string TimeBandForm::windowTitle()
{
    return "Time Band Details";
}

void TimeBandForm::populateGrid()
{
    std::map<std::string, std::string> dayparts;
    dayparts["daypart1"] = mTimeBand->daypart1()->valueToString();
    dayparts["daypart2"] = mTimeBand->daypart2()->valueToString();
    dayparts["daypart3"] = mTimeBand->daypart3()->valueToString();
    dayparts["daypart4"] = mTimeBand->daypart4()->valueToString();
    dayparts["daypart5"] = mTimeBand->daypart5()->valueToString();
    dayparts["daypart6"] = mTimeBand->daypart6()->valueToString();
    dayparts["daypart7"] = mTimeBand->daypart7()->valueToString();
    mDPG->updateGrid(dayparts);
}

void TimeBandForm::populateEntityFields()
{
    mTimeBand->setName(ui->edtName->text().toStdString());
    bool ff = ui->rbFlatFee->isChecked();

    mTimeBand->setBillMethod([ff](){return (ff) ? "FF": "IP";}());

    mTimeBand->setBasePrice(ui->edtBasePrice->value());
    mTimeBand->setBaseDuration(ui->edtBaseDuration->value());
    mTimeBand->setLocked(ui->cbLock->isChecked());

    mTimeBand->setIPDuration1(ui->sbD1->value());
    mTimeBand->setPercent1(ui->sbP1->value());

    mTimeBand->setIPDuration2(ui->sbD2->value());
    mTimeBand->setPercent2(ui->sbP2->value());

    mTimeBand->setIPDuration3(ui->sbD3->value());
    mTimeBand->setPercent3(ui->sbP3->value());

    mTimeBand->setIPDuration4(ui->sbD4->value());
    mTimeBand->setPercent4(ui->sbP4->value());

    mTimeBand->setIPDuration5(ui->sbD5->value());
    mTimeBand->setPercent5(ui->sbP5->value());

    mTimeBand->setIPDuration6(ui->sbD6->value());
    mTimeBand->setPercent6(ui->sbP6->value());

    mTimeBand->setIPDuration7(ui->sbD7->value());
    mTimeBand->setPercent7(ui->sbP7->value());

    mTimeBand->setIPDuration8(ui->sbD8->value());
    mTimeBand->setPercent8(ui->sbP8->value());

    mTimeBand->setIPDuration9(ui->sbD9->value());
    mTimeBand->setPercent9(ui->sbP9->value());

    mTimeBand->setIPDuration10(ui->sbD10->value());
    mTimeBand->setPercent10(ui->sbP10->value());

    mTimeBand->setIPDuration11(ui->sbD11->value());
    mTimeBand->setPercent11(ui->sbP11->value());

    mTimeBand->setIPDuration12(ui->sbD12->value());
    mTimeBand->setPercent12(ui->sbP12->value());


    auto dayparts = mDPG->readGrid();
    mTimeBand->setDaypart1(dayparts["daypart1"]);
    mTimeBand->setDaypart2(dayparts["daypart2"]);
    mTimeBand->setDaypart3(dayparts["daypart3"]);
    mTimeBand->setDaypart4(dayparts["daypart4"]);
    mTimeBand->setDaypart5(dayparts["daypart5"]);
    mTimeBand->setDaypart6(dayparts["daypart6"]);
    mTimeBand->setDaypart7(dayparts["daypart7"]);

}

void TimeBandForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mTimeBand->name()->value()));
    ui->edtBasePrice->setValue(mTimeBand->basePrice()->value());
    ui->edtBaseDuration->setValue(mTimeBand->baseDuration()->value());

    qDebug() << mTimeBand->locked()->value();

    ui->cbLock->setChecked(mTimeBand->locked()->value());

    if (mTimeBand->billMethod()->value() == "FF"){
        ui->rbFlatFee->setChecked(true);
        setEditState(false);
    }else{
        ui->rbIncPercent->setChecked(true);
    }
}

void TimeBandForm::onIncPercent(bool checked)
{
    setEditState(checked);
}

void TimeBandForm::setEditState(bool state)
{
    ui->sbD1->setEnabled(state);
    ui->sbP1->setEnabled(state);

    ui->sbD2->setEnabled(state);
    ui->sbP2->setEnabled(state);

    ui->sbD3->setEnabled(state);
    ui->sbP3->setEnabled(state);

    ui->sbD4->setEnabled(state);
    ui->sbP4->setEnabled(state);

    ui->sbD5->setEnabled(state);
    ui->sbP5->setEnabled(state);

    ui->sbD6->setEnabled(state);
    ui->sbP6->setEnabled(state);

    ui->sbD7->setEnabled(state);
    ui->sbP7->setEnabled(state);

    ui->sbD8->setEnabled(state);
    ui->sbP8->setEnabled(state);

    ui->sbD9->setEnabled(state);
    ui->sbP9->setEnabled(state);

    ui->sbD10->setEnabled(state);
    ui->sbP10->setEnabled(state);

    ui->sbD11->setEnabled(state);
    ui->sbP11->setEnabled(state);

    ui->sbD12->setEnabled(state);
    ui->sbP12->setEnabled(state);

}
