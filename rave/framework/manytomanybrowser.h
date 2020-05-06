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
            const std::string typeInfo,
            BaseEntity* pEnt,
            BaseEntity* dEnt,
            QVBoxLayout* layout,
            QWidget *parent = nullptr);

    ~ManyToManyBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    static ManyToMany*
    createMToM(const std::string typeInfo, BaseEntity* pEnt, BaseEntity* dEnt)
    {
        if (typeInfo == "voice_type_exclusion"){
            VoiceExclusion* ve = new VoiceExclusion(
                        pEnt,dEnt);
            return ve;
        }

        return nullptr;
    }

private slots:
    void selectedItem(BaseEntity* entity);

private:
    Ui::ManyToManyBrowser *ui;
    PickListBrowser* plb;
    BaseEntity* mParentEntity;
    BaseEntity* mDetailEntity;
};

#endif // MANYTOMANYBROWSER_H
