#include "voiceoverform.h"
#include "ui_voiceoverform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"

#include "voiceover.h"
#include "typeexclusion.h"
#include "../framework/picklistbrowser.h"
#include "../framework/valuelist.h"

VoiceOverForm::VoiceOverForm(){}

VoiceOverForm::VoiceOverForm(
        VoiceOver* vo,
        QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::VoiceOverForm),
    mVoiceOver{vo},
    mDayPart{},
    mVoiceEx{},
    mMtoMBrowser{},
    mGenderModel{}
{
    ui->setupUi(bui->baseContainer);
    bindWidgets();
    setTitle(title());
    mDayPart = new DayPartGrid(ui->vlDayPart);
    populateGrid();

    mVoiceEx = new VoiceExclusion(mVoiceOver, new TypeExclusion());
    mMtoMBrowser = new ManyToManyBrowser(mVoiceEx, ui->vlTypeEx, this);

    connect(ui->cbGender, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboChanged(int)));
}

VoiceOverForm::~VoiceOverForm()
{
    qDebug() << "VoiceOverForm::dtor";
    delete mDayPart;
    //delete mVoiceEx;
    //delete mGenderModel;
    //delete mMtoM;
    delete ui;
}

ActionResult VoiceOverForm::saveRecord()
{
    mVoiceOver->populateEntity();

    auto dayparts = mDayPart->readGrid();
    mVoiceOver->setDaypart1(dayparts["daypart1"]);
    mVoiceOver->setDaypart2(dayparts["daypart2"]);
    mVoiceOver->setDaypart3(dayparts["daypart3"]);
    mVoiceOver->setDaypart4(dayparts["daypart4"]);
    mVoiceOver->setDaypart5(dayparts["daypart5"]);
    mVoiceOver->setDaypart6(dayparts["daypart6"]);
    mVoiceOver->setDaypart7(dayparts["daypart7"]);

    ActionResult ar = mVoiceOver->validate();
    return ar;
}

void VoiceOverForm::bindWidgets()
{
    mVoiceOver->name()->setWidget(ui->edtName);
    mVoiceOver->mobileno()->setWidget(ui->edtMobile);
    mVoiceOver->gender()->setWidget(ui->cbGender);

    mGenderModel = new EntityDataModel(new Gender());
    mVoiceOver->gender()->setDataModel(mGenderModel);
    mGenderModel->all();
}

std::string VoiceOverForm::title()
{
    return "Voice Over Details";
}

void VoiceOverForm::populateGrid()
{
    std::map<std::string, std::string> dayparts;
    dayparts["daypart1"] = mVoiceOver->daypart1()->valueToString();
    dayparts["daypart2"] = mVoiceOver->daypart2()->valueToString();
    dayparts["daypart3"] = mVoiceOver->daypart3()->valueToString();
    dayparts["daypart4"] = mVoiceOver->daypart4()->valueToString();
    dayparts["daypart5"] = mVoiceOver->daypart5()->valueToString();
    dayparts["daypart6"] = mVoiceOver->daypart6()->valueToString();
    dayparts["daypart7"] = mVoiceOver->daypart7()->valueToString();
    mDayPart->updateGrid(dayparts);
}


ManyToMany* VoiceOverForm::getMtoM() const
{
    return mVoiceEx;
}

void VoiceOverForm::comboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbGender->model());
    mVoiceOver->gender()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
