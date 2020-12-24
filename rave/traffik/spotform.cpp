#include "spotform.h"
#include "ui_spotform.h"
#include "ui_baseentitydetaildlg.h"
#include "../utils/daypartgrid.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/manytomanybrowser.h"
#include "../framework/manytomany.h"
#include "../utils/tools.h"

#include "client.h"
#include "spot.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"

SpotForm::SpotForm(Client* client, Spot* spot,
                   QDialog *parent) :
    BaseEntityDetailDlg{parent},
    ui{new Ui::SpotForm},
    m_client{client},
    m_spot{spot}
{
    ui->setupUi(bui->baseContainer);
    setTitle(windowTitle());
    populateGrid();
    populateFormWidgets();

    m_MtoMVoiceOverBrowser =
            std::make_unique<ManyToManyBrowser>(&m_spot->spotVoiceOver(),
                                                ui->vlVoiceOver,
                                                this);

    m_MtoMTypeExBrowser =
            std::make_unique<ManyToManyBrowser>(&m_spot->spotTypeExclusion(),
                                                ui->vlTypeEx,
                                                this);

    connect(ui->cbBrands, QOverload<int>::of(&QComboBox::currentIndexChanged),
           this, &SpotForm::brandsComboChanged);

    ui->tabWidget->setCurrentIndex(0);
}

SpotForm::~SpotForm()
{
    delete ui;
}

ActionResult SpotForm::saveRecord()
{
    populateEntityFields();
    return m_spot->validate();
}

std::vector<EntityRecord> const& SpotForm::voiceOvers() const
{
    return m_MtoMVoiceOverBrowser->entityDataModel().modelEntities();
}

std::vector<EntityRecord> const& SpotForm::typeExclusions() const
{
    return m_MtoMTypeExBrowser->entityDataModel().modelEntities();
}

int SpotForm::parentId() const
{
    return m_spot->id();
}

std::string SpotForm::windowTitle()
{
    return "Client Spot - "+m_client->name()->displayName();
}

void SpotForm::populateGrid()
{
    m_daypart = std::make_unique<DayPartGrid>(ui->vlDaypart);

    std::map<int, std::string> dayparts;
    dayparts[1] = m_spot->daypart1()->valueToString();
    dayparts[2] = m_spot->daypart2()->valueToString();
    dayparts[3] = m_spot->daypart3()->valueToString();
    dayparts[4] = m_spot->daypart4()->valueToString();
    dayparts[5] = m_spot->daypart5()->valueToString();
    dayparts[6] = m_spot->daypart6()->valueToString();
    dayparts[7] = m_spot->daypart7()->valueToString();

    m_daypart->update_grid(dayparts);
}

void SpotForm::populateEntityFields()
{
    m_spot->setName(ui->edtName->text().toStdString());
    m_spot->setSpotDuration(ui->edtSpotDuration->value());
    m_spot->setRealDuration(ui->edtRealDuration->value());
    m_spot->setBrand(m_client->id());
    m_spot->setClient(m_client->id());

    auto dayparts = m_daypart->read_grid();
    m_spot->setDaypart1(dayparts[1]);
    m_spot->setDaypart2(dayparts[2]);
    m_spot->setDaypart3(dayparts[3]);
    m_spot->setDaypart4(dayparts[4]);
    m_spot->setDaypart5(dayparts[5]);
    m_spot->setDaypart6(dayparts[6]);
    m_spot->setDaypart7(dayparts[7]);

}

void SpotForm::populateFormWidgets()
{
    ui->edtName->setText(stoq(m_spot->name()->value()));
    ui->edtSpotDuration->setValue(m_spot->spotDuration()->value());
    ui->edtRealDuration->setValue(m_spot->realDuration()->value());

    ui->cbBrands->setModel(m_spot->brand()->dataModel());
    ui->edtClient->setText(stoq(m_client->name()->value()));
}

void SpotForm::brandsComboChanged(int i)
{
    EntityDataModel* edm = dynamic_cast<EntityDataModel*>(ui->cbBrands->model());
    //auto index = std::get<1>(*(edm->vecBegin()+i))->id();
    //qDebug() << "Brand index: " << index;
    //m_spot->brand()->setValue(index);
    m_spot->brand()->setValue(std::get<1>(*(edm->vecBegin()+i))->id());
}

