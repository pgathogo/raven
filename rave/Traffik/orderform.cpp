#include "orderform.h"
#include "../framework/baseentitydetaildlg.h"
#include "../framework/choicefield.h"
#include "../framework/ravensetup.h"
#include "../framework/ui_baseentitydetaildlg.h"
#include "../utils/tools.h"
#include "ui_orderform.h"

#include "client.h"
#include "order.h"

OrderForm::OrderForm(Client *client, Order *order, QDialog *parent)
    : BaseEntityDetailDlg(parent), ui(new Ui::OrderForm), mClient{client},
      mOrder{order} {
  ui->setupUi(bui->baseContainer);
  setTitle(windowTitle());

  populateFormWidgets();

  connect(ui->cbBrand, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &OrderForm::brandCBChanged);

  connect(ui->cbAgency, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &OrderForm::agencyCBChanged);

  connect(ui->cbAccountRep, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &OrderForm::accountRepCBChanged);

  connect(ui->cbPackage, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &OrderForm::packageRepCBChanged);

  connect(ui->cbRevenueType,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::revenueTypeCBChanged);

  connect(ui->cbBillingType,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::billingTypeCBChanged);

  connect(ui->cbBillingPeriod,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::billingPeriodCBChanged);

  connect(ui->cbBillingBasis,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::billingBasisCBChanged);

  connect(ui->cbAgencyCommType,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::agencyCommTypeCBChanged);

  connect(ui->cbSaleRepCommType,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::saleRepCommTypeCBChanged);

  connect(ui->cbTradeCreditType,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &OrderForm::tradeCreditTypeCBChanged);

  dmSetup = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
  dmSetup->all();
  mSetup = dynamic_cast<RavenSetup *>(dmSetup->firstEntity().get());

  if (mOrder->isNew()) {
    prepare_new_order();
  } else {
    prepare_for_edit(mOrder);
  }
}

OrderForm::~OrderForm() { delete ui; }

ActionResult OrderForm::saveRecord() {
  populateEntityFields();
  auto ar = mOrder->validate();

  if (std::get<0>(ar) == ActionResultType::arSUCCESS) {
    dmSetup->updateEntity(*mSetup);
    dmSetup->all();
  }

  return ar;
}

std::string OrderForm::windowTitle() { return "Order Details Form"; }

void OrderForm::populateEntityFields() {
  mOrder->setTitle(ui->edtTitle->text().toStdString());
  mClient->set_name(ui->edtClient->text().toStdString());

  mOrder->setOrderNumber(ui->sbOrderNumber->value());
  mOrder->setOrderDate(ui->dtOrderDate->date());
  mOrder->setStartDate(ui->dtStartDate->date());
  mOrder->setEndDate(ui->dtEndDate->date());

  mOrder->setSpotsOrdered(ui->sbSOrdered->value());
  mOrder->setSpotsBooked(ui->sbSBooked->value());
  mOrder->setSpotsPlayed(ui->sbSPlayed->value());

  mOrder->setDiscount(ui->sbDiscount->value());

  mOrder->setAgencyComm(ui->sbAgencyComm->value());

  mOrder->setSalesRepComm(ui->sbSaleRepComm->value());

  mOrder->setTradeCredit(ui->sbTradeCredit->value());

  mOrder->setGracePeriod(ui->sbGracePeriod->value());
  mOrder->setLateFee(ui->sbLateFee->value());

  mOrder->setAddLogin("postgres");
  mOrder->setAddDtime(QDateTime::currentDateTime());

  setChoiceField(mOrder->tradeCreditType(), ui->cbTradeCreditType);
  setChoiceField(mOrder->salesRepCommType(), ui->cbSaleRepCommType);
  setChoiceField(mOrder->agencyCommType(), ui->cbAgencyCommType);
  setChoiceField(mOrder->revenueType(), ui->cbRevenueType);
  setChoiceField(mOrder->billingType(), ui->cbBillingType);
  setChoiceField(mOrder->billingPeriod(), ui->cbBillingPeriod);
  setChoiceField(mOrder->billingBasis(), ui->cbBillingBasis);

  // setChoiceFieldDefault(QComboBox *cbox, const std::string val)
}

void OrderForm::populateFormWidgets() {
  ui->edtTitle->setText(stoq(mOrder->title()->value()));
  ui->edtClient->setText(stoq(mClient->name()->value()));

  ui->cbBrand->setModel(mOrder->brand()->dataModel());
  ui->cbAgency->setModel(mOrder->agency()->dataModel());
  ui->cbAccountRep->setModel(mOrder->accountRep()->dataModel());
  ui->cbPackage->setModel(mOrder->package()->dataModel());

  ui->sbOrderNumber->setValue(mOrder->orderNumber()->value());
  ui->dtOrderDate->setDate(mOrder->orderDate()->value());
  ui->dtStartDate->setDate(mOrder->startDate()->value());
  ui->dtEndDate->setDate(mOrder->endDate()->value());

  ui->sbSOrdered->setValue(mOrder->spotsOrdered()->value());
  ui->sbSBooked->setValue(mOrder->spotsBooked()->value());

  qDebug() << "Spots Played: " << mOrder->spotsPlayed()->value();
  ui->sbSPlayed->setValue(mOrder->spotsPlayed()->value());

  populateChoiceCombo(ui->cbRevenueType, mOrder->revenueType());
  populateChoiceCombo(ui->cbBillingType, mOrder->billingType());
  populateChoiceCombo(ui->cbBillingPeriod, mOrder->billingPeriod());
  populateChoiceCombo(ui->cbBillingBasis, mOrder->billingBasis());

  ui->sbDiscount->setValue(mOrder->discount()->value());

  ui->sbAgencyComm->setValue(mOrder->agencyComm()->value());
  populateChoiceCombo(ui->cbAgencyCommType, mOrder->agencyCommType());

  ui->sbSaleRepComm->setValue(mOrder->salesRepComm()->value());
  populateChoiceCombo(ui->cbSaleRepCommType, mOrder->salesRepCommType());

  ui->sbTradeCredit->setValue(mOrder->tradeCredit()->value());
  populateChoiceCombo(ui->cbTradeCreditType, mOrder->tradeCreditType());

  ui->sbGracePeriod->setValue(mOrder->gracePeriod()->value());
  ui->sbLateFee->setValue(mOrder->lateFee()->value());
}
void OrderForm::clear_widgets() {}

void OrderForm::populateChoiceCombo(QComboBox *cbox,
                                    const ChoiceField<std::string> *cf) {
  for (const auto &c : cf->choices())
    cbox->addItem(stoq(std::get<1>(c)), stoq(std::get<0>(c)));

  setChoiceFieldDefault(cbox, cf->value());
  // cbox->setCurrentIndex( cbox->findData(QVariant(stoq(cf->value()))) );
}

void OrderForm::prepare_new_order() {
  mSetup->orderNumberSequence()->setValue(
      mSetup->orderNumberSequence()->value() + 1);
  ui->sbOrderNumber->setValue(mSetup->orderNumberSequence()->value());

  ui->edtClient->setText(stoq(mClient->name()->value()));

  setDefaults();
}

void OrderForm::prepare_for_edit(Order *order) {
  // If any spots have been booked from this order, don't edit.
  if (order->spotsBooked()->value() > 0) {
    disable_controls();
  }

  // If spots have been played from this order, don't edit.
  if (order->spotsPlayed()->value() > 0) {
    qDebug() << "* disable controls *";
    disableSaveBtn();
  }
}

void OrderForm::disable_controls() {
  auto widgets = this->findChildren<QWidget *>();
  for (auto w : widgets) {
    w->setEnabled(false);
  }

  bui->btnClose->setEnabled(true);

  QLabel* l = new QLabel("This order has spots booked. Editing is disabled.");
  l->setStyleSheet("color: red;");
  bui->baseContainer->layout()->addWidget(l);

  // ui->sbSOrdered->setEnabled(false);
}

void OrderForm::setDefaults() {

  ui->sbAgencyComm->setValue(mSetup->agencyComm()->value());
  setChoiceFieldDefault(ui->cbAgencyCommType,
                        mSetup->agencyCommType()->value());

  ui->sbSaleRepComm->setValue(mSetup->saleRepComm()->value());
  setChoiceFieldDefault(ui->cbSaleRepCommType,
                        mSetup->saleRepCommType()->value());

  setChoiceFieldDefault(ui->cbRevenueType, mSetup->revenueType()->value());

  setChoiceFieldDefault(ui->cbBillingType, mSetup->billingType()->value());
  setChoiceFieldDefault(ui->cbBillingPeriod, mSetup->billingCycle()->value());
  setChoiceFieldDefault(ui->cbBillingBasis, mSetup->billingBasis()->value());

  ui->sbGracePeriod->setValue(mSetup->gracePeriod()->value());
  ui->sbLateFee->setValue(mSetup->lateFee()->value());

  ui->cbBrand->setCurrentIndex(-1);
  ui->cbAgency->setCurrentIndex(-1);
  ui->cbAccountRep->setCurrentIndex(-1);
  ui->cbPackage->setCurrentIndex(-1);

  ui->dtOrderDate->setDate(QDate::currentDate());
  ui->dtStartDate->setDate(QDate::currentDate());
  ui->dtEndDate->setDate(QDate::currentDate());
}

void OrderForm::setChoiceFieldDefault(QComboBox *cbox, const std::string val) {
  cbox->setCurrentIndex(cbox->findData(QVariant(stoq(val))));
}

void OrderForm::setForeignKeyField(QComboBox *cbox, ForeignKeyField *fkf) {
  EntityDataModel *edm = dynamic_cast<EntityDataModel *>(cbox->model());
  fkf->setValue(std::get<1>(*(edm->vecBegin() + cbox->currentIndex()))->id());
}

void OrderForm::revenueTypeCBChanged(int i) {
  mOrder->revenueType()->setValue(
      ui->cbRevenueType->itemData(i).toString().toStdString());
}

void OrderForm::billingTypeCBChanged(int i) {
  mOrder->billingType()->setValue(
      ui->cbBillingType->itemData(i).toString().toStdString());
}
void OrderForm::billingPeriodCBChanged(int i) {
  mOrder->billingPeriod()->setValue(
      ui->cbBillingPeriod->itemData(i).toString().toStdString());
}
void OrderForm::billingBasisCBChanged(int i) {
  mOrder->billingBasis()->setValue(
      ui->cbBillingBasis->itemData(i).toString().toStdString());
}

void OrderForm::brandCBChanged(int i) {
  if (i < 0)
    return;

  EntityDataModel *edm = dynamic_cast<EntityDataModel *>(ui->cbBrand->model());
  mOrder->brand()->setValue(std::get<1>(*(edm->vecBegin() + i))->id());
}

void OrderForm::agencyCBChanged(int i) {
  if (i < 0)
    return;
  EntityDataModel *edm = dynamic_cast<EntityDataModel *>(ui->cbAgency->model());
  mOrder->agency()->setValue(std::get<1>(*(edm->vecBegin() + i))->id());
}

void OrderForm::accountRepCBChanged(int i) {
  if (i < 0)
    return;
  EntityDataModel *edm =
      dynamic_cast<EntityDataModel *>(ui->cbAccountRep->model());
  mOrder->accountRep()->setValue(std::get<1>(*(edm->vecBegin() + i))->id());
}

void OrderForm::packageRepCBChanged(int i) {
  if (i < 0)
    return;
  EntityDataModel *edm =
      dynamic_cast<EntityDataModel *>(ui->cbPackage->model());
  mOrder->package()->setValue(std::get<1>(*(edm->vecBegin() + i))->id());
}

void OrderForm::agencyCommTypeCBChanged(int i) {
  mOrder->agencyCommType()->setValue(
      ui->cbAgencyCommType->itemData(i).toString().toStdString());
}

void OrderForm::saleRepCommTypeCBChanged(int i) {
  mOrder->salesRepCommType()->setValue(
      ui->cbSaleRepCommType->itemData(i).toString().toStdString());
}

void OrderForm::tradeCreditTypeCBChanged(int i) {
  mOrder->tradeCreditType()->setValue(
      ui->cbTradeCreditType->itemData(i).toString().toStdString());
}
