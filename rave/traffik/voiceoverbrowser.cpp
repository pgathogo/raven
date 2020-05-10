#include "voiceoverbrowser.h"
#include "ui_voiceoverbrowser.h"

#include "voiceover.h"
#include "voiceoverform.h"
#include "typeexclusion.h"
#include "../framework/manytomany.h"


VoiceOverBrowser::VoiceOverBrowser(QWidget *parent):
    BaseEntityBrowserDlg(parent, new VoiceOver()),
    ui(new Ui::VoiceOverBrowser),
    mVoiceOver{new VoiceOver()},
    mVOForm{nullptr},
    edm{}
{
    ui->setupUi(this);
    setDialogTitle("Voice Overs");
}

VoiceOverBrowser::~VoiceOverBrowser()
{
    //delete mVoiceOver;
    delete mVOForm;
    delete edm;
    delete ui;
}

void VoiceOverBrowser::addRecord()
{
    mVoiceOver = new VoiceOver();

    if (mVOForm != nullptr)
        delete mVOForm;

    mVOForm = new VoiceOverForm(mVoiceOver, this);

    if (mVOForm->exec() > 0){
        if (entityDataModel()->createEntity(mVoiceOver) ) {

            edm = new EntityDataModel();

            auto iterB = mVOForm->getMtoM()->cVecBegin();
            auto iterE = mVOForm->getMtoM()->cVecEnd();

            for (; iterB != iterE; ++iterB){
                VoiceExclusion* ve = new VoiceExclusion(
                            new VoiceOver(),
                            new TypeExclusion());
                ve->setParentId(mVoiceOver->id());
                ve->setDetailId(dynamic_cast<TypeExclusion*>((*iterB).get())->id());

                edm->createEntity(ve);
            }
         }
    }
}

void VoiceOverBrowser::updateRecord()
{
   update<VoiceOver, VoiceOverForm>();
}
