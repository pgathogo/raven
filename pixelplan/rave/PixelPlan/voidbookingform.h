
#include <QDialog>

class QComboBox;
class QTextEdit;
class QCheckBox;

struct Reason {
    int reason_id;
    bool is_other_reason;
    std::string other_reason;
};

namespace PIXELPLAN
{
    class VoidBookingForm: public QDialog
    {
        Q_OBJECT

    public:
        explicit VoidBookingForm(const QString, QDialog* parent=nullptr);
        Reason void_reason();

    private slots:
        void reason_changed(int);
        void other_selected(int);
        void ok_clicked(bool);

    private:
        void make_widgets();

        QComboBox* m_cbReasons;
        QCheckBox* m_cbOther;
        QTextEdit*  m_edtOther;

        Reason m_reason;
        QString m_void_type;
    };



}
