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
class QListWidgetItem;

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
        virtual std::string windowTitle() = 0;
        virtual void clear_widgets();


        std::string title();
        void closeEvent(QCloseEvent* event);
        BrowserForms const& get_forms() const;

        template<typename T, typename... TArgs>
        T* createMtoM(TArgs... mArgs){
            //static_assert(std::is_base_of<BaseEntityBrowserDlg, T>::value, "`T` must be derived from BaseEntityBrowserDlg");
            auto uPtr = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
            auto ptr(uPtr.get());
            mForms.emplace_back(std::move(uPtr));
            return ptr;
        }

        template<typename T>
        void register_input_widget(T* edit)
        {
            connect(edit, &T::textChanged, this, [&](){m_okay_to_close = false;});
        }


signals:
       void dialog_is_closing();

private slots:
    void btnSaveClicked();
    void btnCloseClicked();
    void btnSaveNewClicked();
    void onCloseDialog();

protected:
    Ui::BaseEntityDetailDlg* bui;
    void save(BaseEntity* entity);
    void setTitle(std::string title);

    void disableSaveBtn();
    void hideSaveBtn();
    void disableSaveNewBtn();
    void hideSaveNewBtn();

private:
   NotificationBar* mNoticeBar;
   bool m_okay_to_close;
   std::vector<std::unique_ptr<ManyToManyBrowser>> mForms;

};

#endif // BASEENTITYDETAILDLG_H
