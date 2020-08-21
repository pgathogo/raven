#ifndef BASEENTITYDETAILDLG_H
#define BASEENTITYDETAILDLG_H

#include <memory>

#include <QDialog>
#include <QKeyEvent>

#include "../utils/types.h"
#include "manytomanybrowser.h"

//class EntityDataModel;
class BaseEntity;
class NotificationBar;

namespace Ui {
class BaseEntityDetailDlg;
}

using BrowserForms = std::vector<std::unique_ptr<ManyToManyBrowser>>;

class BaseEntityDetailDlg : public QDialog
{
    Q_OBJECT

public:
        explicit BaseEntityDetailDlg(QDialog *parent = nullptr);
        ~BaseEntityDetailDlg();

        void connectSlots();
        [[nodiscard]] virtual ActionResult saveRecord()=0;

        virtual void populateEntityFields()=0;
        virtual void populateFormWidgets()=0;

        std::string title();
        virtual std::string windowTitle() = 0;

        void closeEvent(QCloseEvent* event);

        template<typename T, typename... TArgs>
        T* createMtoM(TArgs... mArgs){
            //static_assert(std::is_base_of<BaseEntityBrowserDlg, T>::value, "`T` must be derived from BaseEntityBrowserDlg");
            auto uPtr = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
            auto ptr(uPtr.get());
            mForms.emplace_back(std::move(uPtr));
            return ptr;
        }

       BrowserForms const& getForms() const;

private slots:
    void btnSaveClicked();
    void btnCloseClicked();
    void btnSaveNewClicked();

protected:
    Ui::BaseEntityDetailDlg* bui;
    void save(BaseEntity* entity);
    void setTitle(std::string title);

private:
   NotificationBar* mNoticeBar;
   bool mOkClose;
   std::vector<std::unique_ptr<ManyToManyBrowser>> mForms;
};

#endif // BASEENTITYDETAILDLG_H
