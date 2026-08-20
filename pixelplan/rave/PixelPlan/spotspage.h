#ifndef SPOTSPAGE_H
#define SPOTSPAGE_H

#include <QWizardPage>

class QTableView;
class EntityDataModel;

namespace  PIXELPLAN
{
    class OrderBookingWizard;

    class SpotsPage : public QWizardPage
    {
        Q_OBJECT
    public:
        SpotsPage(OrderBookingWizard*);

        bool validatePage() override;

        inline static QString name {"spots_page"};

        void print() { qDebug() << "I am a: " << name; }

    private slots:
        void on_spot_clicked(const QModelIndex&);

    private:
        void setup_ui();
        void populate_spots_table(int);

        QTableView* m_tvspots;
        std::unique_ptr<EntityDataModel> m_spot_edm;

        OrderBookingWizard* m_wizard;
    };

}


#endif