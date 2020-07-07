#include "timebandbrowser.h"
#include "ui_timebandbrowser.h"

#include "timeband.h"
#include "timebandform.h"

#include "../framework/ravenexception.h"
#include "../utils/tools.h"

TimeBandBrowser::TimeBandBrowser(QWidget* parent) :
    BaseEntityBrowserDlg(parent, new TimeBand),
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
    std::unique_ptr<TimeBand> tband = std::make_unique<TimeBand>();
    std::unique_ptr<TimeBandForm> tbandForm =
            std::make_unique<TimeBandForm>(tband.get());
    if (tbandForm->exec() > 0){
        try{
        entityDataModel()->createEntity(std::move(tband));
        }
        catch(RavenException& re){
            showMessage(re.errorMessage());
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
