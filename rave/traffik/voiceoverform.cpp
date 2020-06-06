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
#include "../utils/tools.h"

VoiceOverForm::VoiceOverForm(){}

VoiceOverForm::VoiceOverForm(
        VoiceOver* vo,
        QDialog *parent) :
    BaseEntityDetailDlg(parent),
    ui(new Ui::VoiceOverForm),
    mVoiceOver{vo},
    mDayPart{},
    mMtoMBrowser{},
    mGenderModel{},
    mVoiceExModel{}
{
    ui->setupUi(bui->baseContainer);
    setTitle(title());
    mDayPart = new DayPartGrid(ui->vlDayPart);
    populateGrid();
    populateFormWidgets();

    mMtoMBrowser = new ManyToManyBrowser(mVoiceOver->voiceEx(),
                                             ui->vlTypeEx, this);

    mVoiceExModel = new EntityDataModel(
                new VoiceExclusion(new VoiceOver, new TypeExclusion));

    connect(ui->cbGender, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboChanged(int)));
}

VoiceOverForm::~VoiceOverForm()
{
    delete mDayPart;
    delete mVoiceExModel;
    qDebug() << "VoiceOverForm::dtor";
    //delete mGenderModel;
    //delete mMtoM;
    delete ui;
}

ActionResult VoiceOverForm::saveRecord()
{

    populateEntityFields();

    // ManyToMany
    if (mVoiceOver->id() > 0)
        saveVoiceExclusions();

    ActionResult ar = mVoiceOver->validate();
    return ar;
}

void VoiceOverForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(mVoiceOver->name()->value()));
    ui->edtMobile->setText(stoq(mVoiceOver->mobileno()->value()));
}

void VoiceOverForm::populateEntityFields()
{
    mVoiceOver->name()->setValue(ui->edtName->text().toStdString());
    mVoiceOver->mobileno()->setValue(ui->edtMobile->text().toStdString());

    auto dayparts = mDayPart->readGrid();
    mVoiceOver->setDaypart1(dayparts["daypart1"]);
    mVoiceOver->setDaypart2(dayparts["daypart2"]);
    mVoiceOver->setDaypart3(dayparts["daypart3"]);
    mVoiceOver->setDaypart4(dayparts["daypart4"]);
    mVoiceOver->setDaypart5(dayparts["daypart5"]);
    mVoiceOver->setDaypart6(dayparts["daypart6"]);
    mVoiceOver->setDaypart7(dayparts["daypart7"]);
}

void VoiceOverForm::saveVoiceExclusions()
{
    VecIter it =  mMtoMBrowser->entityDataModel()->vecBegin();
    VecIter vend = mMtoMBrowser->entityDataModel()->vecEnd();
    for(; it!= vend; ++it){
        ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(*it).get());

        if (mtom->dbAction() == DBAction::dbaCREATE)
            mVoiceExModel->createEntityDB(mtom);

        if (mtom->dbAction() == DBAction::dbaDELETE)
            mVoiceExModel->deleteEntity(mtom);
    }
}

std::string VoiceOverForm::windowTitle()
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
    return mVoiceOver->voiceEx();
}

void VoiceOverForm::comboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbGender->model());
    mVoiceOver->gender()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}
