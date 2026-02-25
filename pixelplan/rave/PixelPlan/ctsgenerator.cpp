#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTabWidget>
#include <QFile>

#include "../../../rave/framework/entitydatamodel.h"
#include "../../../utils/tools.h"

#include "ravensetup.h"
#include "ctsgenerator.h"
#include "formatselectorwidget.h"


namespace PIXELPLAN {

    CTSGenerator::CTSGenerator(QDate date, BookedAdverts& ba, BreakTitles& break_titles, QWidget* parent)
     : QDialog(parent)
    ,m_date{date}
    ,m_booked_adverts{ba}
    ,m_break_titles{break_titles}
    {
       setWindowTitle("CTS Generator");
       setMinimumSize(500, 300);

       setup_ui();
    }

    void CTSGenerator::setup_ui()
    {
       QVBoxLayout* layout = new QVBoxLayout(this);

       m_tabwidget = new QTabWidget(this);

       // Create header settings tab
       m_header_widget = new FormatSelectorWidget("Header Format Settings", this);
       m_tabwidget->addTab(m_header_widget, "Header");

       // Create time seetings tab
       m_time_widget = new FormatSelectorWidget("Time Format Settings", this);
       m_tabwidget->addTab(m_time_widget, "Time Labels");

       m_break_widget = new FormatSelectorWidget("Break Labels Format Settings", this);
       m_tabwidget->addTab(m_break_widget, "Breaks Labels");

       m_spot_widget =  new FormatSelectorWidget("Spot Labels Format Settings", this);
       m_tabwidget->addTab(m_spot_widget, "Spots Labels");

       m_duration_widget = new FormatSelectorWidget("Duration Labels Format Settings", this);
       m_tabwidget->addTab(m_duration_widget, "Duration Labels");

       layout->addWidget(m_tabwidget);
       // Button layout
       QHBoxLayout* button_layout = new QHBoxLayout();

       //QPushButton* btnLoad = new QPushButton("Load Settings...", this);
       QPushButton* btnSave = new QPushButton("Generate", this);
       QPushButton* btnCancel = new QPushButton("Cancel", this);

       button_layout->addStretch();
       button_layout->addWidget(btnSave);
       button_layout->addWidget(btnCancel);

       //connect(btnLoad, &QPushButton::clicked, this, &CTSGenerator::onLoadClicked);
       connect(btnSave, &QPushButton::clicked, this, &CTSGenerator::onSaveClicked);
       connect(btnCancel, &QPushButton::clicked, this, &CTSGenerator::onCancelClicked);

       layout->addLayout(button_layout);

       m_edm_setup = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
       m_edm_setup->all();

       if (m_edm_setup->count() > 0) {
           m_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());
       }

       load_from_file("cts.json");

    }

    QJsonObject CTSGenerator::toJson() const
    {
        QJsonObject json;

        json["header"] = m_header_widget->toJson();

        json["time"] = m_time_widget->toJson();
        json["break"] = m_break_widget->toJson();
        json["spot"] = m_spot_widget->toJson();
        json["duration"] = m_duration_widget->toJson();

        return json;
    }

    void CTSGenerator::fromJson(const QJsonObject& json)
    {
        if (json.contains("header") && json["header"].isObject()) {
            m_header_widget->fromJson(json["header"].toObject());
        }

        if (json.contains("time") && json["time"].isObject()) {
            m_time_widget->fromJson(json["time"].toObject());
        }

        if (json.contains("break") && json["break"].isObject()) {
            m_break_widget->fromJson(json["break"].toObject());
        }

        if (json.contains("spot") && json["spot"].isObject()) {
            m_spot_widget->fromJson(json["spot"].toObject());
        }

        if (json.contains("duration") && json["duration"].isObject()) {
            m_duration_widget->fromJson(json["duration"].toObject());
        }

    }


    bool CTSGenerator::save_to_file(const QString& filename) const
    {
        QJsonObject json = toJson();
        QJsonDocument doc(json);

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(nullptr, "Error", QString("Could not save to file: %1").arg(filename));
            return false;
        }

        file.write(doc.toJson());
        file.close();
        return true;
    }

    bool CTSGenerator::load_from_file(const QString& filename)
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(nullptr, "Error", QString("Could not load from file: %1").arg(filename));
            return false;
        }

        QByteArray data = file.readAll();
        file.close();

        QJsonDocument  doc = QJsonDocument::fromJson(data);
        if(doc.isNull() || !doc.isObject()) {
            QMessageBox::warning(nullptr, "Error", "Invalid JSON format in file");
            return false;
        }

        fromJson(doc.object());
        return true;
    }

    void CTSGenerator::onSaveClicked()
    {
        // QString filename = QFileDialog::getSaveFileName(this, "Save Format settings",
        //                                                 "", "JSON Files (*.json)");

        QString filename = "cts.json";

        if (!filename.isEmpty()) {
            if (save_to_file(filename)) {
                qDebug() << "Settings saved successfully...";
            }
        }

        // Generate CTS file
        if (generate_cts()) {
            qDebug() << "CTS file generated successfully...";
            done(1);
        } else {
            done(0);
        }

    }

    void CTSGenerator::onCancelClicked()
    {
        done(0);
    }

    void CTSGenerator::onLoadClicked()
    {
        QString filename = QFileDialog::getOpenFileName(this,
                                                        "Load Format settings", "", "JSON Files (*.json)");
        if (!filename.isEmpty()) {
            if (load_from_file(filename)) {
                QMessageBox::information(this, "Success", "Settings loaded successfully!");
            }
        }
    }

    CTSElement CTSGenerator::make_cts_element(CTSElementType element_type, const QString& value)
    {
        CTSElement cts_element;

        switch(element_type) {
        case CTSElementType::Header: {
                cts_element.element = xls_string(header_settings(), value);
                cts_element.header_space = header_settings()->getHeaderSpacing();
                cts_element.footer_space = header_settings()->getFooterSpacing();
                break;
        }
        case CTSElementType::Time: {
                cts_element.element = xls_string(time_settings(), value);
                cts_element.header_space = time_settings()->getHeaderSpacing();
                cts_element.footer_space = time_settings()->getFooterSpacing();
                break;
                break;
        }
        case CTSElementType::Break: {
                cts_element.element = xls_string(break_settings(), value);
                cts_element.header_space = break_settings()->getHeaderSpacing();
                cts_element.footer_space = break_settings()->getFooterSpacing();
                break;
        }
        case CTSElementType::Spot: {
                cts_element.element = xls_string(spot_settings(), value);
                cts_element.header_space = spot_settings()->getHeaderSpacing();
                cts_element.footer_space = spot_settings()->getFooterSpacing();
                break;
        }
        case CTSElementType::Duration: {
                cts_element.element = xls_string(duration_settings(), value);
                cts_element.header_space = duration_settings()->getHeaderSpacing();
                cts_element.footer_space = duration_settings()->getFooterSpacing();
                break;
        }
        }

        return cts_element;
    }

    QXlsx::RichString CTSGenerator::xls_string(FormatSelectorWidget* fsw, const QString& value)
    {
        QXlsx::RichString rich_string;
        QXlsx::Format string_format;

        QColor bgc = fsw->getBackgroundColor();
        if (bgc != QColor("#ffffff"))
            string_format.setPatternBackgroundColor(fsw->getBackgroundColor());

        string_format.setFontBold(fsw->getFont().bold());
        string_format.setFontItalic(fsw->getFont().italic());

        if (fsw->getAlignment() == "left") {
            string_format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
        } else if (fsw->getAlignment() == "Center") {
            string_format.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        } else if (fsw->getAlignment() == "Right" ) {
            string_format.setHorizontalAlignment(QXlsx::Format::AlignRight);
        }

        rich_string.addFragment(value, string_format);

        return rich_string;
    }

    bool CTSGenerator::generate_cts()
    {
        QXlsx::Document xlsx;
        xlsx.setColumnWidth(3, 50.0);

        int row = 1;
        int col = 3;

        QStringList days_of_week = {"MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY", "SUNDAY"};

        QString dow = days_of_week.at(m_date.dayOfWeek()-1);

        QString header_title = dow + ": " + m_date.toString("dd/MM/yyyy");

        CTSElement header = make_cts_element(CTSElementType::Header, header_title);
        xlsx.write(row, col, header.element);

        row = header.footer_space+1;

        for (auto& [time, adverts] : m_booked_adverts) {

            col = 2;
            auto time_elem = make_cts_element(CTSElementType::Time, time.toString("hh:mm"));

            xlsx.write(row, col, time_elem.element);

            col = 3;
            auto break_elem = make_cts_element(CTSElementType::Break, m_break_titles[time]);
            xlsx.write(row, col, break_elem.element);

            row = row + break_elem.footer_space+1;
            int spot_footer = 0;
            for (auto& advert : adverts) {

                col = 3;
                auto spot_elem = make_cts_element(CTSElementType::Spot, advert.spot_name);
                xlsx.write(row, col, spot_elem.element);

                if (advert.duration > 0) {
                    col = 4;
                    auto duration_elem = make_cts_element(CTSElementType::Duration, duration_to_time(advert.duration).toString("hh:mm:ss"));
                    xlsx.write(row++, col, duration_elem.element);
                }

                spot_footer = spot_elem.footer_space;
            }

            xlsx.write(row, col, "");

            row = row + spot_footer;

        }

        QString filepath = dow+"_"+m_date.toString("ddMMyyyy")+".xlsx";

        QString cts_filepath{};

        if (!m_setup->cts_filepath()->value().empty()) {
            cts_filepath = m_setup->cts_filepath()->to_qstring() +"/"+filepath;
        } else {
            cts_filepath = filepath;
        }

        if (xlsx.saveAs(cts_filepath)) {
            showQMessage("CTS file created successfully: " + cts_filepath, QMessageBox::Information);
        } else {
            showQMessage("Failed to create Excel file: " + cts_filepath, QMessageBox::Warning);
        }

        return true;
    }


}
