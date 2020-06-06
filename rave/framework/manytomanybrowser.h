#ifndef MANYTOMANYBROWSER_H
#define MANYTOMANYBROWSER_H

#include <QWidget>

#include "baseentitybrowserdlg.h"
#include "manytomany.h"
#include "typeexclusion.h"

class EntityDataModel;
class QVBoxLayout;
class PickListBrowser;

namespace Ui {
class ManyToManyBrowser;
}

class ManyToManyBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "ManyToManyBrowser";

    explicit ManyToManyBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~ManyToManyBrowser() override;

    void addRecord() override;
    void updateRecord() override;
    void deleteRecord() override;

    std::string typeID() override;

    //createMToM(const std::string typeInfo, BaseEntity* pEnt, BaseEntity* dEnt)
    static ManyToMany*
    createMtoM(ManyToMany* mtom)
    {
        if (mtom->typeInfo() == "voice_type_exclusion"){
            VoiceExclusion* ve = new VoiceExclusion(
                        mtom->parentEntity(),
                        mtom->detailEntity());
            return ve;
        }
        return nullptr;
    }

    ManyToMany* createMtoM(ManyToMany* mtom, BaseEntity* detail)
    {
        if (mtom->typeInfo() == "voice_type_exclusion"){
            TypeExclusion* dte = dynamic_cast<TypeExclusion*>(detail);
            //TypeExclusion te(*dte);
            VoiceExclusion* ve = new VoiceExclusion(
                        mtom->parentEntity(), dte);
            ve->setDBAction(detail->dbAction());
            return ve;
        }
        return nullptr;

    }

    size_t cnt(){ return mMtoM->getSize(); }
    ManyToMany* getManyToMany() const{ return mMtoM; }

private slots:
    void selectedItem(BaseEntity* entity);

private:
    Ui::ManyToManyBrowser *ui;
    PickListBrowser* plb;
    ManyToMany* mMtoM;
    //BaseEntity* mParentEntity;
    //BaseEntity* mDetailEntity;
};

#endif // MANYTOMANYBROWSER_H
