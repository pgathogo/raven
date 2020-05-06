#ifndef MANYTOMANYBROWSER_H
#define MANYTOMANYBROWSER_H

#include <QWidget>

#include "baseentitybrowserdlg.h"
#include "manytomany.h"

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
    explicit ManyToManyBrowser(
            ManyToMany* mtom,
            QVBoxLayout* layout,
            QWidget *parent = nullptr
            );

    ~ManyToManyBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    //createMToM(const std::string typeInfo, BaseEntity* pEnt, BaseEntity* dEnt)
    static ManyToMany*
    createMToM(ManyToMany* mtom)
    {
        if (mtom->typeInfo() == "voice_type_exclusion"){
            VoiceExclusion* ve = new VoiceExclusion(
                        mtom->parentEntity(),
                        mtom->detailEntity());
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
