
#include "../../../rave/framework/ui_baseentitydetaildlg.h"

#include "voidbookingreasonform.h"
#include "ui_voidbookingreasonform.h"

#include "reasonvoidbooking.h"



VoidBookingReasonForm::VoidBookingReasonForm(PIXELPLAN::ReasonVoidBooking* rvb,
                                             QDialog *parent)
    :BaseEntityDetailDlg(parent)
    ,ui(new Ui::VoidBookingReasonForm)
    ,m_reason_void_booking{rvb}
{
    ui->setupUi(bui->baseContainer);

    setTitle(windowTitle());
    populateFormWidgets();
}

VoidBookingReasonForm::~VoidBookingReasonForm()
{
    delete ui;
}

ActionResult VoidBookingReasonForm::saveRecord()
{
    populateEntityFields();
    return ActionResult();
}

void VoidBookingReasonForm::populateFormWidgets()
{
    ui->edtReason->setText(m_reason_void_booking->reason()->to_qstring());
    ui->edtDesc->setText(m_reason_void_booking->description()->to_qstring());

}

void VoidBookingReasonForm::populateEntityFields()
{
    m_reason_void_booking->reason()->setValue(ui->edtReason->text().toStdString());
    m_reason_void_booking->description()->setValue(ui->edtDesc->toPlainText().toStdString());
}


std::string VoidBookingReasonForm::windowTitle()
{
    return "Void Booking Reason";
}

void VoidBookingReasonForm::clear_widgets()
{

}
