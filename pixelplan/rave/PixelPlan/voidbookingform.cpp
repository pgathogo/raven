#include <algorithm>
#include <cctype>  // Required for std::isspace

#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QMessageBox>

#include "voidbookingform.h"
#include "orderbooking.h"

#include "../../../rave/framework/entitydatamodel.h"

namespace PIXELPLAN
{

VoidBookingForm::VoidBookingForm(const QString void_type, QDialog* parent)
{
    m_void_type = void_type;

    make_widgets();
    m_edtOther->setEnabled(false);

    QString title = QString("Reason to %1 Booking").arg(m_void_type);
    setWindowTitle(title);
}

void VoidBookingForm::make_widgets()
{
    QString lbl = QString("%1 Reason:").arg(m_void_type);
    QLabel* lbl_reason = new QLabel(lbl);
    m_cbReasons = new QComboBox();

    OrderBooking ob;
    m_cbReasons->setModel(ob.void_reason()->dataModel());

    // connect(m_cbReasons, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //         this, &VoidBookingForm::reason_changed);

    m_cbReasons->setCurrentIndex(0);

    m_cbOther = new QCheckBox("Other");
    connect(m_cbOther, &QCheckBox::stateChanged, this, &VoidBookingForm::other_selected);

    QLabel* lblOther = new QLabel("Other Reason:");
    m_edtOther = new QTextEdit();

    QVBoxLayout* vlMain = new QVBoxLayout();

    QGridLayout* grid_layout = new QGridLayout();

    grid_layout->addWidget(lbl_reason, 0, 0);    // Row 0 Column 0
    grid_layout->addWidget(m_cbReasons, 0, 1);   // Row 0 Column 1
    grid_layout->setColumnStretch(1, 1);

    QHBoxLayout* hlCheckBox = new QHBoxLayout();
    hlCheckBox->addWidget(m_cbOther);

    QGridLayout* gridOther = new QGridLayout();
    gridOther->addWidget(lblOther, 0, 0);
    gridOther->addWidget(m_edtOther, 0, 1);

    QPushButton* btn_save = new QPushButton("Ok");
    QPushButton* btn_cancel = new QPushButton("Cancel");

    connect(btn_save, &QPushButton::clicked, this, &VoidBookingForm::ok_clicked);
    connect(btn_cancel, &QPushButton::clicked, this, &VoidBookingForm::close);

    QHBoxLayout* hl_buttons = new QHBoxLayout();
    hl_buttons->addStretch(1);
    hl_buttons->addWidget(btn_save);
    hl_buttons->addWidget(btn_cancel);

    vlMain->addLayout(grid_layout);
    vlMain->addLayout(hlCheckBox);
    vlMain->addLayout(gridOther);
    vlMain->addLayout(hl_buttons);

    setLayout(vlMain);

}
void VoidBookingForm::reason_changed(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(m_cbReasons->model());
    m_reason.reason_id  = std::get<1>(*(edm->vecBegin()+i))->id();
}

void VoidBookingForm::other_selected(int state)
{
    m_reason.reason_id = -1;
    bool is_other = (state == Qt::Unchecked) ? true : false;

    m_cbReasons->setEnabled(is_other);
    m_edtOther->setEnabled(!is_other);
}

void VoidBookingForm::ok_clicked(bool state)
{
    m_reason.is_other_reason = (m_cbOther->checkState()== Qt::Unchecked) ? false : true;
    m_reason.other_reason = m_edtOther->toPlainText().toStdString();
    reason_changed(m_cbReasons->currentIndex());

    // Check if other reasons - we have comments
    if (m_reason.is_other_reason)
    {
        std::string s = m_reason.other_reason;
        s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char x) { return std::isspace(x); }), s.end());

        if (s.size() == 0) {
            QMessageBox msgbox;
            msgbox.setText("Please enter reason to void booking!");
            msgbox.exec();
            return;
        }

        m_reason.reason_id = -1;
    }

    done(1);
}

Reason VoidBookingForm::void_reason()
{
    return m_reason;
}


}
