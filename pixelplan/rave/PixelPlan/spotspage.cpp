#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QLineEdit>
#include <QHeaderView>
#include <QDebug>

#include "../../../rave/framework/entitydatamodel.h"
#include "spot.h"

#include "spotspage.h"
#include "orderbookingwizard.h"

namespace PIXELPLAN
{

SpotsPage::SpotsPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Client Spot Selection");
    setSubTitle("Select spot to book");

    setup_ui();
    populate_spots_table(1);
}

void SpotsPage::setup_ui()
{
    QVBoxLayout* main_layout = new QVBoxLayout();

    m_tvspots = new QTableView();

    m_tvspots->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(m_tvspots, &QTableView::clicked, this, &SpotsPage::on_spot_clicked);

    QHBoxLayout* search_layout = new QHBoxLayout();

    QLabel* lbl_spot_name = new QLabel("Spot Name:");
    QLineEdit* edt_spot_name = new QLineEdit();

    search_layout->addWidget(lbl_spot_name);
    search_layout->addWidget(edt_spot_name);

    main_layout->addWidget(m_tvspots);
    main_layout->addLayout(search_layout);

    setLayout(main_layout);

}

void SpotsPage::populate_spots_table(int client_id)
{
    m_spot_edm = std::make_unique<EntityDataModel>(
        std::make_shared<TRAFFIK::Spot>()
        );

    auto spot = std::make_unique<TRAFFIK::Spot>();
    auto spot_filter = std::make_tuple(
                           spot->client()->dbColumnName(),
                           " = ",
                           client_id
        );

    m_spot_edm->search(m_spot_edm->prepareFilter(spot_filter));

    m_tvspots->setModel(m_spot_edm.get());

}

void SpotsPage::on_spot_clicked(const QModelIndex& index)
{
    if (!index.isValid()) return;

    const QAbstractItemModel* abs_model = index.model();
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(const_cast<QAbstractItemModel*>(abs_model));

    auto entity = edm->get_entity_at(index.row());

    m_wizard->booking_data.spot = dynamic_pointer_cast<TRAFFIK::Spot>(entity);

}

bool SpotsPage::validatePage()
{
    if (m_wizard->booking_data.spot == nullptr) {
        showMessage("Please select a spot", QMessageBox::Information);
        return false;
    }

}

} // namespace