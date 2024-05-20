#ifndef BOOKEDCOMMERCIALDLG_H
#define BOOKEDCOMMERCIALDLG_H

#include <QDialog>

namespace DATA{
   class CommercialViewer;
}

namespace Ui {
    class BookedCommercialDlg;
}

class BookedCommercialDlg : public QDialog
{
    Q_OBJECT

public:
    explicit BookedCommercialDlg(QWidget* parent = nullptr);
    ~BookedCommercialDlg();
    int show_booked_commercial(int);

public slots:
    void close_dialog();

private:
    Ui::BookedCommercialDlg *ui;
    std::unique_ptr<DATA::CommercialViewer> m_comm_viewer;
};

#endif // BOOKEDCOMMERCIALDLG_H
