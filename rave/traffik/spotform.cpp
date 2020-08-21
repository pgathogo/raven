#include "spotform.h"
#include "ui_spotform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"
#include "../utils/tools.h"

#include "client.h"
#include "spot.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"

SpotForm::SpotForm(Client* client, Spot* spot,
                   QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::SpotForm),
    mClient{client},
    mSpot{spot}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateGrid();
    populateFormWidgets();

    mMtoMVoiceOverBrowser =
            std::make_unique<ManyToManyBrowser>(&mSpot->spotVoiceOver(),
                                                ui->vlVoiceOver,
                                                this);

    mMtoMTypeExBrowser =
            std::make_unique<ManyToManyBrowser>(&mSpot->spotTypeExclusion(),
                                                ui->vlTypeEx,
                                                this);


    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    ui->tabWidget->setCurrentIndex(0);
}

SpotForm::~SpotForm()
{
    delete ui;
}

ActionResult SpotForm::saveRecord()
{
    populateEntityFields();
    return mSpot->validate();
}

std::vector<EntityRecord> const& SpotForm::voiceOvers() const
{
    return mMtoMVoiceOverBrowser->entityDataModel().modelEntities();
}

std::vector<EntityRecord> const& SpotForm::typeExclusions() const
{
    return mMtoMTypeExBrowser->entityDataModel().modelEntities();
}

int SpotForm::parentId() const
{
    return mSpot->id();
}

std::string SpotForm::windowTitle()
{
    return "Client Spot - "+mClient->name()->displayName();
}

void SpotForm::populateGrid()
{
    mDaypart = std::make_unique<DayPartGrid>(ui->vlDaypart);

    std::map<std::string, std::string> dayparts;
    dayparts["daypart1"] = mSpot->daypart1()->valueToString();
    dayparts["daypart2"] = mSpot->daypart2()->valueToString();
    dayparts["daypart3"] = mSpot->daypart3()->valueToString();
    dayparts["daypart4"] = mSpot->daypart4()->valueToString();
    dayparts["daypart5"] = mSpot->daypart5()->valueToString();
    dayparts["daypart6"] = mSpot->daypart6()->valueToString();
    dayparts["daypart7"] = mSpot->daypart7()->valueToString();

    mDaypart->updateGrid(dayparts);

}

void SpotForm::populateEntityFields()
{
    mSpot->setName(ui->edtName->text().toStdString());
    mSpot->setSpotDuration(ui->edtSpotDuration->value());
    mSpot->setRealDuration(ui->edtRealDuration->value());
    mSpot->setBrand(mClient->id());
    mSpot->setClient(mClient->id());

    auto dayparts = mDaypart->readGrid();
    mSpot->setDaypart1(dayparts["daypart1"]);
    mSpot->setDaypart2(dayparts["daypart2"]);
    mSpot->setDaypart3(dayparts["daypart3"]);
    mSpot->setDaypart4(dayparts["daypart4"]);
    mSpot->setDaypart5(dayparts["daypart5"]);
    mSpot->setDaypart6(dayparts["daypart6"]);
    mSpot->setDaypart7(dayparts["daypart7"]);


}

void SpotForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mSpot->name()->value()));
    ui->edtSpotDuration->setValue(mSpot->spotDuration()->value());
    ui->edtRealDuration->setValue(mSpot->realDuration()->value());

    ui->cbBrands->setModel(mSpot->brand()->dataModel());
    ui->edtClient->setText(stoq(mClient->name()->value()));

}

void SpotForm::brandsComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrands->model());
    mSpot->brand()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());

}

