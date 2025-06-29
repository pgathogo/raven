#include "timebandbrowser.h"
#include "ui_timebandbrowser.h"

#include "timeband.h"
#include "timebandform.h"

#include "../../../rave/framework/ravenexception.h"
#include "../../../rave/utils/tools.h"

TimeBandBrowser::TimeBandBrowser(QWidget* parent) :
    BaseEntityBrowserDlg(parent,
                         std::make_shared<TimeBand>()),
    ui(new Ui::TimeBandBrowser)
{
    ui->setupUi(this);
    setDialogTitle("Time Bands");
}

TimeBandBrowser::~TimeBandBrowser()
{
    delete ui;
}

void TimeBandBrowser::addRecord()
{
    auto tband = std::make_shared<TimeBand>();

    std::unique_ptr<TimeBandForm> tbandForm =
            std::make_unique<TimeBandForm>(tband.get());

    if (tbandForm->exec() > 0){
        try{
        entityDataModel().createEntity(std::move(tband));
        }
        catch(DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }
}

void TimeBandBrowser::updateRecord()
{
    update<TimeBand, TimeBandForm>();
}

std::string TimeBandBrowser::typeID()
{
    return "TimeBandBrowser";
}
