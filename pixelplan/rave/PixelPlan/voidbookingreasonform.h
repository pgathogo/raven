#ifndef VOIDBOOKINGREASONFORM_H
#define VOIDBOOKINGREASONFORM_H

#include <QWidget>
#include "../../../rave/framework/baseentitydetaildlg.h"

namespace PIXELPLAN {
    class ReasonVoidBooking;
}

namespace Ui {
class VoidBookingReasonForm;
}

class VoidBookingReasonForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit VoidBookingReasonForm(PIXELPLAN::ReasonVoidBooking* rvb,
                                   QDialog *parent = nullptr);
    ~VoidBookingReasonForm();

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void clear_widgets() override;

    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::VoidBookingReasonForm *ui;
    PIXELPLAN::ReasonVoidBooking* m_reason_void_booking;
};

#endif // VOIDBOOKINGREASONFORM_H
