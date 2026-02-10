#include "voidreasonbrowser.h"
#include "ui_voidreasonbrowser.h"

#include "voidreasonbrowser.h"
#include "reasonvoidbooking.h"

#include "voidbookingreasonform.h"
#include "reasonvoidbooking.h"

VoidReasonBrowser::VoidReasonBrowser(QWidget *parent) :
    BaseEntityBrowserDlg(parent, std::make_shared<PIXELPLAN::ReasonVoidBooking>()),
    ui(new Ui::VoidReasonBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Reason Void Booking");
}

VoidReasonBrowser::~VoidReasonBrowser()
{
    delete ui;
}

void VoidReasonBrowser::addRecord()
{
    std::shared_ptr<PIXELPLAN::ReasonVoidBooking> rvb = std::make_shared<PIXELPLAN::ReasonVoidBooking>();
    std::unique_ptr<VoidBookingReasonForm> rvb_form = std::make_unique<VoidBookingReasonForm>(rvb.get());

    if (rvb_form->exec() > 0)
        entityDataModel().createEntity(std::move(rvb));


}

void VoidReasonBrowser::updateRecord()
{
    update<PIXELPLAN::ReasonVoidBooking, VoidBookingReasonForm>();

}

std::string VoidReasonBrowser::typeID()
{
    return "VoidReasonBrowser";
}
