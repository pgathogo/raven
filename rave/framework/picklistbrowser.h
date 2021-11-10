#ifndef PICKLISTBROWSER_H
#define PICKLISTBROWSER_H

#include <QDialog>

#include "baseentitybrowserdlg.h"

class SelectCloseWidget;
class ManyToMany;
class EntityDataModel;

namespace Ui {
class PickListBrowser;
}

enum class PickListMode{
    plmSINGLE_SELECT,
    plmMULTIPLE_SELECT
};

struct PickListSetting {
    PickListSetting(std::unique_ptr<BaseEntity> entity, int search_col_index,
                    PickListMode plm = PickListMode::plmSINGLE_SELECT);
    std::unique_ptr<BaseEntity> listEntity;
    std::vector<BaseEntity*> selectedEntities;
    PickListMode pickMode;
    int m_search_col_index=0;
};


class PickListBrowser : public BaseEntityBrowserDlg
{
    Q_OBJECT

public:
    static constexpr auto TYPEID = "PickListBrowser";

                //EntityDataModel* edm,
    explicit PickListBrowser(PickListSetting& plSetting,
                            QWidget *parent = nullptr);
    ~PickListBrowser() override;

    void addRecord() override;
    void updateRecord() override;

    int get_column_index(std::string);
    std::string typeID() override;
public slots:
    void onSelectItem();
    void onCloseSelection();

signals:
    void itemSelected(BaseEntity* entity);

private:
    Ui::PickListBrowser *ui;
    SelectCloseWidget* scw;
    PickListSetting& m_picklist_setting;
};

#endif // PICKLISTBROWSER_H
