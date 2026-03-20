#ifndef PLAYLISTFORM_H
#define PLAYLISTFORM_H

#include <QDialog>
#include "bookedcommercialtvm.h"
#include "playlist.h"
#include "configmanager.h"

#define UPPER(c)   (((c) >= 'a' && (c) <= 'z') ? (c) - 'a' + 'A' : (c))

namespace Ui {
class PlaylistForm;
}

class QMdiArea;
class RavenSetup;

using SCHAR = signed char;
using UCHAR = unsigned char;
using SLONG = signed long;
using ULLONG = unsigned long long;

constexpr size_t HASH_SIZE = 211;

class PlaylistForm : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistForm(QWidget* parent = nullptr);
    ~PlaylistForm();

    void setMdiArea(QMdiArea*);

public slots:
    void date_changed(const QDate&);
    void create_playlist_file(bool);
    void generate_cts();
    void test_hash();


private:
    BookedAdverts get_booked_adverts(QDate);
    void display_booked_adverts(BookedAdverts&);
    void set_comm_break_attr(std::string, PIXELPLAN::PlaylistItemAttributes&);
    void set_advert_attr(const BookedAdvert&, PIXELPLAN::PlaylistItemAttributes&);
    QString estimated_entry_time();
    QString file_modification_time();
    QString date_template_set();
    QString generate_guid();
    QString generateDeterministicGuid(const QString&, const QString&);
    QString file_id(const QString&);
    int hash(const SCHAR*);
    uint64_t hash19(const char*);
    unsigned int basicHash(const UCHAR*);
    unsigned long long hash64(const char*);

    void expand_all_with_children();
    void expand_if_has_children(QModelIndex);
    QDate previous_weekday(int current_dow);
    QString make_playlist_output_filepath(int);

    QString replace_relative_path(QString);
    bool confirm_playlist_filename(QString&);

    Ui::PlaylistForm *ui;

    QMdiArea* m_mdi_area;


    std::unique_ptr<EntityDataModel> m_edm_setup;
    std::shared_ptr<RavenSetup> m_setup;
    BookedAdverts m_booked_adverts;
    BreakTitles m_break_titles;
    int m_items_order = 10000000;

    PIXELPLAN::ConfigManager m_config_manager;
    QString m_media_path_tag;
    QString m_media_path_rep;

    static int comm_header_id;

};

#endif // PLAYLISTFORM_H
