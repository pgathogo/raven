#include "voiceoverbrowser.h"
#include "ui_voiceoverbrowser.h"

#include "voiceover.h"
#include "voiceoverform.h"
#include "typeexclusion.h"
#include "../framework/manytomany.h"


VoiceOverBrowser::VoiceOverBrowser(QWidget *parent):
    BaseEntityBrowserDlg(parent, new VoiceOver())
    ,ui(new Ui::VoiceOverBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Voice Overs");
}

VoiceOverBrowser::~VoiceOverBrowser()
{
    delete ui;
}

void VoiceOverBrowser::addRecord()
{
    std::unique_ptr<VoiceOver> vo =
            std::make_unique<VoiceOver>();

    std::unique_ptr<VoiceOverForm> voForm =
           std::make_unique<VoiceOverForm>(vo.get(), this);

    if (voForm->exec() > 0){
        if (entityDataModel()->createEntity(vo.get()) ) {

            std::unique_ptr<EntityDataModel> edm =
                   std::make_unique<EntityDataModel>();

            auto& exlusions = voForm->typeExclusions();
            for(auto& type : exlusions){

                ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(type).get());
                if (mtom->dbAction() == DBAction::dbaCREATE){
                    mtom->setParentId(vo->id());
                    edm->createEntityDB(mtom);
                }

                if (mtom->dbAction() == DBAction::dbaDELETE)
                    edm->deleteEntity(mtom);

            }
         }
    }

}

void VoiceOverBrowser::updateRecord()
{
   update<VoiceOver, VoiceOverForm>();
}

void VoiceOverBrowser::deleteRecord()
{
    BaseEntity* entity = findSelectedEntity();
    EntityDataModel edm(new VoiceExclusion);
    edm.deleteEntityByValue({"parent_id", entity->id()});
    entityDataModel()->deleteEntity(entity);
    removeSelectedRow();
}

std::string VoiceOverBrowser::typeID()
{
    return "VoiceOverBrowser";
}
