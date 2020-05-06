#include "voiceoverbrowser.h"
#include "ui_voiceoverbrowser.h"

#include "voiceover.h"
#include "voiceoverform.h"

VoiceOverBrowser::VoiceOverBrowser(QWidget *parent,
                                   VoiceOver* vo) :
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
    mVOForm = new VoiceOverForm(mVoiceOver);
    if (mVOForm->exec() > 0)
        entityDataModel()->createEntity(mVoiceOver);
}

void VoiceOverBrowser::updateRecord()
{
   update<VoiceOver, VoiceOverForm>();
}
