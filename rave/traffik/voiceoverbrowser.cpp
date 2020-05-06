#include "voiceoverbrowser.h"
#include "ui_voiceoverbrowser.h"

#include "voiceover.h"
#include "voiceoverform.h"
#include "typeexclusion.h"
#include "../framework/manytomany.h"


VoiceOverBrowser::VoiceOverBrowser(QWidget *parent):
    BaseEntityBrowserDlg(parent, new VoiceOver()),
    ui(new Ui::VoiceOverBrowser),
    mVoiceOver{new VoiceOver()}
{
    ui->setupUi(this);
    setDialogTitle("Voice Overs");
}

VoiceOverBrowser::~VoiceOverBrowser()
{
    //delete mVoiceOver;
    delete mVOForm;
    delete ui;
}

void VoiceOverBrowser::addRecord()
{
    mVoiceOver = new VoiceOver();
    mVOForm = new VoiceOverForm(mVoiceOver, this);
    if (mVOForm->exec() > 0){
       // entityDataModel()->createEntity(mVoiceOver);
        auto iterB = mVOForm->getMtoM()->cVecBegin();
        auto iterE = mVOForm->getMtoM()->cVecEnd();
        for (; iterB != iterE; ++iterB){
            TypeExclusion* te = dynamic_cast<TypeExclusion*>((*iterB).get());
            qDebug() << QString::fromStdString(te->name()->valueToString());
        }
    }
}

void VoiceOverBrowser::updateRecord()
{
   update<VoiceOver, VoiceOverForm>();
}
