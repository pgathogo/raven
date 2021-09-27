#include <QComboBox>
#include <QDebug>
#include <QHeaderView>
#include "saveas.h"
#include "ui_saveas.h"

SaveAs::SaveAs(const std::vector<int> hours, QWidget *parent)
    :m_selected_hours{hours}
    ,QDialog(parent)
    ,ui(new Ui::SaveAs)
{
    ui->setupUi(this);

    setWindowTitle("Save As...");

    setup_selection_table();
    ui->dtSchedule->setDate(QDate::currentDate());

    connect(ui->btnSave, &QPushButton::clicked, this, &SaveAs::save);
    connect(ui->btnCancel, &QPushButton::clicked, this, &SaveAs::cancel);
}

SaveAs::~SaveAs()
{
    delete ui;
}

void SaveAs::save()
{
    m_result.dest_map.clear();

    m_result.dest_date = ui->dtSchedule->date();

    for(int r=0; r < ui->twHours->rowCount(); ++r){
        auto item = ui->twHours->item(r, 0);
        if (item->checkState() == Qt::Checked){
            int hour = item->data(Qt::UserRole).toInt();
            QComboBox* combo = (QComboBox*)ui->twHours->cellWidget(r, 1);
            m_result.dest_map[hour] = combo->currentText().toInt();
        }
    }

    done(1);
}

void SaveAs::cancel()
{
    reject();
}

Result SaveAs::save_result()
{
    return m_result;
}

void SaveAs::setup_selection_table()
{
    QStringList	header;
    ui->twHours->setHorizontalHeaderLabels(header);
    ui->twHours->setRowCount(m_selected_hours.size());

    int row=0;
    for (int hour : m_selected_hours){
        qDebug() << "Hour: "<< hour;
        QTableWidgetItem* tw_item = new QTableWidgetItem(QString::number(hour));
        tw_item->setCheckState(Qt::Checked);
        tw_item->setData(Qt::UserRole, hour);
        ui->twHours->setItem(row, 0,  tw_item);

        QComboBox* cb = new QComboBox();
        add_combo_items(cb);
        cb->setCurrentIndex(cb->findText(QString::number(hour)));
        ui->twHours->setCellWidget(row, 1, cb);
        ++row;
    }

    ui->twHours->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void SaveAs::add_combo_items(QComboBox* cb)
{
    for (int h=0; h < 23; ++h)
        cb->addItem(QString::number(h));
}
