#include <sstream>
#include <iostream>

#include <QDate>
#include <QTimeZone>
#include <QUuid>
#include <QCryptographicHash>
#include <QFileInfo>

#include "../../../rave/framework/entitydatamodel.h"
#include "ravensetup.h"

#include "playlistform.h"
#include "ui_playlistform.h"

int PlaylistForm::comm_header_id = 0;

PlaylistForm::PlaylistForm(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::PlaylistForm)
    , m_mdi_area{nullptr}
    , m_setup{nullptr}
{
    ui->setupUi(this);

    ui->dtStartDate->setDate(QDate::currentDate());

    BookedAdverts ba = get_booked_adverts(QDate::currentDate());
    display_booked_adverts(ba);

    connect(ui->dtStartDate, &QDateEdit::dateChanged, this, &PlaylistForm::on_date_changed);
    connect(ui->btnCreateFile, &QPushButton::clicked, this, &PlaylistForm::on_create_playlist_file);

    m_edm_setup = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
    m_edm_setup->all();

    if (m_edm_setup->count() > 0) {
        m_setup = std::dynamic_pointer_cast<RavenSetup>(m_edm_setup->firstEntity());
    }

    setFixedSize(1020, 480);
    setWindowTitle("View Booked Adverts");

}

PlaylistForm::~PlaylistForm()
{
    delete ui;
}

void PlaylistForm::setMdiArea(QMdiArea* mdi)
{
    m_mdi_area = mdi;
}

void PlaylistForm::on_date_changed(const QDate& prev_date)
{
    m_booked_adverts = get_booked_adverts(ui->dtStartDate->date());


    display_booked_adverts(m_booked_adverts);
}

void PlaylistForm::on_create_playlist_file(bool clicked)
{
    // 1. Get our empty playlist XML file
    // 2. Load the XML file to the DomDocument
    // 3. Loop the QDomDocument with breaks and booked adverts
    // 4. Save the updated QDomDocument to file - "

    m_booked_adverts = get_booked_adverts(ui->dtStartDate->date());

    QMessageBox msgbox;

    QString empty_playlist_filepath = m_setup->playlist_template_filepath()->to_qstring();
    QString output_path = m_setup->playlist_output_path()->to_qstring();

    QString output_filepath = output_path + "/test.ch1_xml";

    qDebug() << "Playlist File: "<< empty_playlist_filepath;
    qDebug() << "Output File  : " << output_filepath;

    if (empty_playlist_filepath.isEmpty()) {
        msgbox.setText("Please set the playlist template file");
        msgbox.exec();
        return;
    }

    PIXELPLAN::Playlist playlist(empty_playlist_filepath, output_filepath);

    auto [status, msg] = playlist.set_playlist_content();

    if (!status) {
        msgbox.setText(msg);
        msgbox.exec();
        return;
    }


    for(auto [time, adverts]: m_booked_adverts)
    {
        auto comm_break_attributes = playlist.attributes();

        std::string t_str = time.toString("hh:mm").toStdString();
        std::string title = "Commercial Break: "+t_str;

        set_comm_break_attr(title, comm_break_attributes);
        playlist.create_playlist_item(comm_break_attributes);

        for(auto& advert : adverts) {
            if (advert.filepath.isEmpty())
                continue;
            auto advert_attributes = playlist.attributes();
            set_advert_attr(advert, advert_attributes);
            playlist.create_playlist_item(advert_attributes);
        }


    }

        auto [play_status, play_msg] = playlist.save_playlist();

        QMessageBox mb;

        if (!play_status) {
            mb.setText("Error saving playlist file: "+ play_msg);
            mb.exec();
            return;
        }

        mb.setText("Playlist saved successfully.");
        mb.exec();
}

void PlaylistForm::set_comm_break_attr(std::string title,
                                       PIXELPLAN::PlaylistItemAttributes& attrs)
{
    PIXELPLAN::PlaylistElement ple;

    --comm_header_id;

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ID;
    attrs[ple.to_string()] = std::to_string(comm_header_id);

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ORD;
    attrs[ple.to_string()] = std::to_string(m_items_order);
    m_items_order += 2500;

    // Break Header
    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ITEM_TYPE;
    attrs[ple.to_string()] = "Comment";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::CLIP_PATH;
    attrs[ple.to_string()] = title;

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ENTRY_TIME_ESTIMATED;
    attrs[ple.to_string()] = estimated_entry_time().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::FILE_MODIFICATION_TIME;
    attrs[ple.to_string()] = file_modification_time().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ANALYZED;
    attrs[ple.to_string()] = "100";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::DATE_TEMPLATE_SET;
    attrs[ple.to_string()] = date_template_set().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::GUID;
    attrs[ple.to_string()] = generate_guid().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::SubtitleEnabled;
    attrs[ple.to_string()] = "1";

}

void PlaylistForm::set_advert_attr(const BookedAdvert& ba, PIXELPLAN::PlaylistItemAttributes& attrs)
{
    PIXELPLAN::PlaylistElement ple;

    --comm_header_id;

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ID;
    attrs[ple.to_string()] = std::to_string(comm_header_id);

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ORD;
    attrs[ple.to_string()] = std::to_string(m_items_order);
    m_items_order += 2500;

    // Break Header
    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ITEM_TYPE;
    attrs[ple.to_string()] = "file";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::CLIP_PATH;
    attrs[ple.to_string()] = ba.filepath.toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ENTRY_TIME_ESTIMATED;
    attrs[ple.to_string()] = estimated_entry_time().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::FILE_MODIFICATION_TIME;
    attrs[ple.to_string()] = file_modification_time().toStdString();

    // -----

    QFileInfo fi(ba.filepath);
    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::FILE_SIZE;
    attrs[ple.to_string()] = std::to_string(fi.size());

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::TOTAL_DURATION;
    attrs[ple.to_string()] = std::to_string(ba.duration * 1000);  //TODO:: Confirm MagicSoft milliseconds calculations!!


    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::EVENT_END;
    attrs[ple.to_string()] = "0";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::MATERIAL_AVAILABILITY;
    attrs[ple.to_string()] = "1";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::VIDEO_WIDTH;
    attrs[ple.to_string()] = "1920";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::VIDEO_HEIGHT;
    attrs[ple.to_string()] = "1080";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ASPECT_RATIO;
    attrs[ple.to_string()] = "1.7778";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::FILE_ID;
    attrs[ple.to_string()] = "83473843847384738";

    // ----

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::ANALYZED;
    attrs[ple.to_string()] = "0";

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::DATE_TEMPLATE_SET;
    attrs[ple.to_string()] = date_template_set().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::GUID;
    attrs[ple.to_string()] = generate_guid().toStdString();

    ple.m_tag_type = PIXELPLAN::PlaylistElement::TagType::SubtitleEnabled;
    attrs[ple.to_string()] = "1";
}

QString PlaylistForm::estimated_entry_time()
{
    // 2. Define the timezone using the constructor with the offset in seconds
    // The offset "+03:00" is equivalent to 3 hours * 60 minutes * 60 seconds.
    int offsetSeconds = 3 * 60 * 60;
    QTimeZone timeZone(offsetSeconds);

    QTime time(0, 0, 0);
    // 3. Create the QDateTime object
    QDateTime dateTime(QDate::currentDate(), time, timeZone);

    // 4. Format the QDateTime object to the desired string
    // The `toIso8601String()` function handles this format correctly,
    // including the time zone offset.
    QString formattedString = dateTime.toString(Qt::ISODate);

    // 5. Print the result
    return formattedString;
}

QString PlaylistForm::file_modification_time()
{
    QDateTime dateTime = QDateTime::currentDateTimeUtc();
    QString isoString = dateTime.toString(Qt::ISODate);
    return isoString;
}

QString PlaylistForm::date_template_set()
{
    QDateTime now = QDateTime::currentDateTime();
    QTimeZone tz(3 * 3600); // +03:00
    now = now.toTimeZone(tz);

    QString datePart = now.toString("yyyy-MM-ddThh:mm:ss");

    // Get current milliseconds and pad to 7 digits
    int msec = now.time().msec();

    QString fractional = QString(".%1").arg(msec, 3, 10, QChar('0'));

    QString offset = now.toString("t").replace("UTC","")+":00";

    return datePart + fractional + offset;

}

QString PlaylistForm::generate_guid()
{
    // 1. Generate a new, unique QUuid.
    QUuid guid = QUuid::createUuid();

    // 2. Convert the QUuid to a string. The standard format is enclosed in curly braces.
    QString guidString = guid.toString();

    // 3. Output the result.
    //ui->lblGuid1->setText(guidString);
    return guid.toString(QUuid::WithoutBraces).toUpper();
}


void PlaylistForm::display_booked_adverts(BookedAdverts& ba)
{
    BookedCommercialTreeViewModel* bctvm = new BookedCommercialTreeViewModel(ba);
    ui->tvPlaylist->setModel(bctvm);
    ui->tvPlaylist->header()->setSectionResizeMode(QHeaderView::Stretch);

    //ui->tvPlaylist->expandRecursively(QModelIndex());
    expand_all_with_children();
}

void PlaylistForm::expand_all_with_children()
{
    qDebug() << "Start expansion....";

    QModelIndex parent_index = QModelIndex(); //ui->tvPlaylist->model()->index(0,0, QModelIndex());

    qDebug() << "Found parent index...";

    expand_if_has_children(parent_index);
}

void PlaylistForm::expand_if_has_children(QModelIndex index)
{
    if (!index.isValid() && ui->tvPlaylist->model()->rowCount(index) == 0){
        return;
    }

    int row_count = ui->tvPlaylist->model()->rowCount(index);

    qDebug() << "Tree rowcount ..." << row_count;

    for(int row=0; row < row_count; ++row)
    {
        QModelIndex child_index = ui->tvPlaylist->model()->index(row, 0, index);
        if (ui->tvPlaylist->model()->hasChildren(child_index)) {
            ui->tvPlaylist->expand(child_index);
            expand_if_has_children(child_index);
        }
    }

}

BookedAdverts PlaylistForm::get_booked_adverts(QDate date)
{
    std::string current_date = date.toString("yyyy-MM-dd").toStdString();
    // Get the commercial breaks

    BookedAdverts booked_adverts;

    std::stringstream sql;

    sql << "SELECT a.schedule_date, a.schedule_time, "
        << " c.id AS spot_id, c.name AS spot_name, c.spot_duration, "
        << " d.name AS client_name, "
        << " e.title, e.media_path, e.file_extension "
        <<  "FROM rave_schedule a "
        << " LEFT OUTER JOIN rave_orderbooking b ON a.id = b.schedule_id "
        << " LEFT JOIN rave_spot c ON b.spot_id = c.id "
        << " LEFT JOIN rave_client d ON c.client_id = d.id "
        << " LEFT JOIN rave_advertmedia e ON b.booked_audio_id = e.id "
        << " WHERE a.schedule_date = '"+current_date+"'"
        << " ORDER by a.schedule_time ASC ";

    EntityDataModel edm;

    edm.readRaw(sql.str());

    auto provider = edm.getDBManager()->provider();

    if (provider->cacheSize() == 0)
        return booked_adverts;

    provider->cache()->first();

    do {
        auto itB = provider->cache()->currentElement()->begin();
        auto itE = provider->cache()->currentElement()->end();

        BookedAdvert ba;

        for (; itB != itE; ++itB)
        {
            std::string field_name = (*itB).first;
            std::string field_value = (*itB).second;


            if (field_name == "schedule_date") {
                QString sdate = QString::fromStdString(field_value);
                ba.booked_date = QDate::fromString(sdate, "yyyy-MM-dd");
            }

            if (field_name == "schedule_time") {
                QString stime = QString::fromStdString(field_value);
                ba.booked_time = QTime::fromString(stime, "hh:mm:ss");
            }

            if (field_name == "spot_id")
                ba.spot_id = str_to_int(field_value);

            if (field_name == "spot_name")
                ba.spot_name = QString::fromStdString(field_value);

            if (field_name == "spot_duration")
                ba.duration = str_to_int(field_value);

            if (field_name == "client_name")
                ba.client_name = QString::fromStdString(field_value);

            if (field_name == "title")
                ba.filename = QString::fromStdString(field_value);

            if (field_name == "media_path")
                ba.media_path = QString::fromStdString(field_value);

            if (field_name == "file_extension")
                ba.file_extension = QString::fromStdString(field_value);

        }


        if (!ba.filename.isEmpty())
            ba.filepath = ba.media_path + ba.filename + "."+ ba.file_extension;

        booked_adverts[ba.booked_time].push_back(ba);

        provider->cache()->next();

    } while(!provider->cache()->isLast());


    return booked_adverts;

}
