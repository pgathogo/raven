#include "voiceoverform.h"
#include "ui_voiceoverform.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"

#include "voiceover.h"
#include "../framework/typeexclusion.h"
#include "timeband.h"

#include "../framework/picklistbrowser.h"
#include "../framework/valuelist.h"
#include "../utils/tools.h"


VoiceOverForm::VoiceOverForm(){}

VoiceOverForm::VoiceOverForm(
        VoiceOver* vo,
        QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::VoiceOverForm),
    mVoiceOver{vo},
    mDayPart{nullptr},
    mMtoMBrowser{nullptr}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());

    setDayPart();

    populateFormWidgets();

    mMtoMBrowser =
            std::make_unique<ManyToManyBrowser>(&mVoiceOver->voiceEx(),
                                                ui->vlTypeEx,
                                                this);

    connect(ui->cbGender, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboChanged(int)));

    ui->tabWidget->setCurrentIndex(0);
}

VoiceOverForm::~VoiceOverForm()
{
    qDebug() << "VoiceOverForm::dtor";
    delete ui;
}

ActionResult VoiceOverForm::saveRecord()
{
    populateEntityFields();

    return mVoiceOver->validate();
}

void VoiceOverForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mVoiceOver->name()->value()));
    ui->edtMobile->setText(stoq(mVoiceOver->mobileno()->value()));
    ui->cbGender->setModel(mVoiceOver->gender()->dataModel());
    ui->cbGender->setCurrentIndex(ui->cbGender->findText(
                                 QString::fromStdString(mVoiceOver->gender()->displayName())));
}

void VoiceOverForm::populateEntityFields()
{
    mVoiceOver->name()->setValue(ui->edtName->text().toStdString());
    mVoiceOver->mobileno()->setValue(ui->edtMobile->text().toStdString());

    auto dayparts = mDayPart->read_grid();

    mVoiceOver->setDaypart1(dayparts[1]);
    mVoiceOver->setDaypart2(dayparts[2]);
    mVoiceOver->setDaypart3(dayparts[3]);
    mVoiceOver->setDaypart4(dayparts[4]);
    mVoiceOver->setDaypart5(dayparts[5]);
    mVoiceOver->setDaypart6(dayparts[6]);
    mVoiceOver->setDaypart7(dayparts[7]);
}

std::vector<EntityRecord> const& VoiceOverForm::typeExclusions() const
{
    return mMtoMBrowser->entityDataModel().modelEntities();
}

std::string VoiceOverForm::windowTitle()
{
    return "Voice Over Details";
}

void VoiceOverForm::setDayPart()
{
   auto edm = std::make_unique<EntityDataModel>(std::make_unique<TimeBand>());
   edm->all();
   TimeBandDayParts tbDaypart;
   for (auto& [name, entity] :edm->modelEntities()){
       TimeBand* tb = dynamic_cast<TimeBand*>(entity.get());
       tbDaypart[tb->name()->displayName()] = tb->dayparts();
   }
   mDayPart = std::make_unique<DayPartGrid>(ui->vlDayPart, tbDaypart);
   populateDayPart(*mDayPart);

}

void VoiceOverForm::populateDayPart(DayPartGrid& dp)
{
    std::map<int, std::string> dayparts;

    dayparts[1] = mVoiceOver->daypart1()->valueToString();
    dayparts[2] = mVoiceOver->daypart2()->valueToString();
    dayparts[3] = mVoiceOver->daypart3()->valueToString();
    dayparts[4] = mVoiceOver->daypart4()->valueToString();
    dayparts[5] = mVoiceOver->daypart5()->valueToString();
    dayparts[6] = mVoiceOver->daypart6()->valueToString();
    dayparts[7] = mVoiceOver->daypart7()->valueToString();

    dp.update_grid(dayparts);
}

void VoiceOverForm::comboChanged(int i)
{
    // Set the gender id with the id of the selected entity from the model.
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbGender->model());
    mVoiceOver->gender()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

int VoiceOverForm::parentId() const
{
    return mVoiceOver->id();
}
