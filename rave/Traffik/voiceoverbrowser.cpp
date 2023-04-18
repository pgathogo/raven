#include "voiceoverbrowser.h"
#include "ui_voiceoverbrowser.h"

#include "voiceover.h"
#include "voiceoverform.h"
#include "../framework/typeexclusion.h"
#include "../framework/manytomany.h"
#include "../framework/ravenexception.h"
#include "../utils/tools.h"

VoiceOverBrowser::VoiceOverBrowser(QWidget *parent):
    BaseEntityBrowserDlg(parent,
                         std::make_unique<VoiceOver>())
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
    auto vo = std::make_shared<VoiceOver>();

    std::unique_ptr<VoiceOverForm> voForm =
           std::make_unique<VoiceOverForm>(vo.get(), this);

    if (voForm->exec() > 0){

        try{
            // create header record
            entityDataModel().createEntity(std::move(vo));

            saveVoiceExclusions(*voForm);

         }
        catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }

}

void VoiceOverBrowser::updateRecord()
{
   auto vof = update<VoiceOver, VoiceOverForm>();
   if (vof != nullptr)
       saveVoiceExclusions(*vof);
}

void VoiceOverBrowser::saveVoiceExclusions(const VoiceOverForm& vof)
{
    std::unique_ptr<EntityDataModel> edm =
           std::make_unique<EntityDataModel>();

    auto& exlusions = vof.typeExclusions();
    for(auto& type : exlusions){
        ManyToMany* mtom = dynamic_cast<ManyToMany*>(std::get<1>(type).get());

        if (mtom->dbAction() == DBAction::dbaCREATE){
            mtom->setParentId(vof.parentId());
            edm->createEntityDB(*mtom);
        }

        if (mtom->dbAction() == DBAction::dbaDELETE)
            edm->deleteEntity(*mtom);
    }
}


void VoiceOverBrowser::deleteRecord()
{
    try{
        std::shared_ptr<BaseEntity> entity = findSelectedEntity();
        EntityDataModel edm(std::make_shared<VoiceExclusion>());
        edm.deleteEntityByValue({"parent_id", entity->id()});
        entityDataModel().deleteEntity(*(entity.get()));
        removeSelectedRow();
    }
    catch(DatabaseException& de){
       showMessage(de.errorMessage());
    }
}

std::string VoiceOverBrowser::typeID()
{
    return "VoiceOverBrowser";
}
