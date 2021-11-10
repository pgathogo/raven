
#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "datatoolbar.h"
#include "artistform.h"
#include "audioform.h"

#include "../audio/audiolibrary.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiolibitem.h"
#include "../audio/artisttypeitem.h"
#include "../audio/genretypeitem.h"
#include "../audio/audiotool.h"
#include "../audio/genreform.h"

#include "../framework/baseentity.h"
#include "../framework/treeviewmodel.h"
#include "../framework/ravenexception.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../framework/relationmapper.h"
#include "../framework/ravenexception.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_artist_toolbar = new DataToolBar();
    ui->hlArtist->addWidget(m_artist_toolbar);

    // Artist Data toolbar
    connect(m_artist_toolbar->add_button(), &QPushButton::clicked, this, &MainWindow::add_artist);
    connect(m_artist_toolbar->edit_button(), &QPushButton::clicked, this, &MainWindow::edit_artist);
    connect(m_artist_toolbar->delete_button(), &QPushButton::clicked, this, &MainWindow::delete_artist);
    connect(ui->btnSearchArtist, &QPushButton::clicked, this, &MainWindow::search_artist);
    connect(ui->edtSearchArtist, &QLineEdit::returnPressed, this, &MainWindow::search_artist);


    // Genre Data toolbar
    m_genre_toolbar = new DataToolBar();
    ui->hlGenre->addWidget(m_genre_toolbar);
    connect(m_genre_toolbar->add_button(), &QPushButton::clicked, this, &MainWindow::add_genre);
    connect(m_genre_toolbar->edit_button(), &QPushButton::clicked, this, &MainWindow::edit_genre);
    connect(m_genre_toolbar->delete_button(), &QPushButton::clicked, this, &MainWindow::delete_genre);

    // Operation buttons
    connect(ui->btnAudioProp, &QPushButton::clicked, this, &MainWindow::audio_properties);

    m_audio_entity_data_model = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());
    m_artist_entity_data_model = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Artist>());
    m_genre_entity_data_model = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Genre>());

    setup_audio_folders();

    m_tracks_model = new QStandardItemModel(this);
    create_track_view_headers();
    set_track_view_column_width();

    m_audio_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_tracks_model);

    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    connect(ui->tvTracks, &QTableView::doubleClicked, this, &MainWindow::track_double_clicked);

    m_artist_model = new QStandardItemModel(this);
    m_artist_type_item = std::make_unique<AUDIO::ArtistTypeItem>(m_artist_model);
    ui->tvArtists->setModel(m_artist_model);
    connect(ui->tvArtists, &QTableView::clicked, this, &MainWindow::artist_selected);

    setup_artists_view();
    fetch_artist();

    m_genre_model = new QStandardItemModel(this);
    ui->tvGenre->setModel(m_genre_model);
    m_genre_type_item = std::make_unique<AUDIO::GenreTypeItem>(m_genre_model);
    ui->tvGenre->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tvGenre, &QTableView::clicked, this, &MainWindow::genre_selected);

    setup_genre_view();
    fetch_genre();

    ui->tabWidget->setCurrentIndex(0);
    setWindowTitle("Raven - Audio Explorer");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_audio_folders()
{
    try{
        AudioLibrary audio_lib;
        auto tree_data = audio_lib.read_data_from_db();
        m_folder_model = new TreeViewModel(tree_data, this);
        ui->tvFolders->setModel(m_folder_model);
        m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));
        connect(ui->tvFolders, &QTreeView::clicked, this, &MainWindow::folder_clicked);
    }catch(DatabaseException& de){
        qDebug() << QString::fromStdString(de.errorMessage());
    }
}

void MainWindow::setup_artists_view()
{
    create_artist_view_headers();
    set_artist_view_columns_width();
}


void MainWindow::create_track_view_headers()
{
    m_tracks_model->setHorizontalHeaderItem(0, new QStandardItem("Track Title"));
    m_tracks_model->setHorizontalHeaderItem(1, new QStandardItem("Artist"));
    m_tracks_model->setHorizontalHeaderItem(2, new QStandardItem("Duration"));
    m_tracks_model->setHorizontalHeaderItem(3, new QStandardItem("Audio Type"));
    m_tracks_model->setHorizontalHeaderItem(4, new QStandardItem("Audio File"));
}

void MainWindow::create_artist_view_headers()
{
    m_artist_model->setHorizontalHeaderItem(0, new QStandardItem("First Name"));
    m_artist_model->setHorizontalHeaderItem(1, new QStandardItem("Surname"));
    m_artist_model->setHorizontalHeaderItem(2, new QStandardItem("Fullname"));
    m_artist_model->setHorizontalHeaderItem(3, new QStandardItem("Artist Type"));
}

void MainWindow::set_artist_view_columns_width()
{
    enum Column{FirstName, Surname, FullName, ArtistType};

    ui->tvArtists->setColumnWidth(Column::FirstName, 200);
    ui->tvArtists->setColumnWidth(Column::Surname, 200);
    ui->tvArtists->setColumnWidth(Column::FullName, 350);
    ui->tvArtists->setColumnWidth(Column::ArtistType, 100);
}

void MainWindow::set_track_view()
{
    m_tracks_model->clear();
    create_track_view_headers();
    set_track_view_column_width();
}

void MainWindow::set_track_view_column_width()
{
    enum Column{Title, Artist, Duration, AudioType, AudioFile};
    ui->tvTracks->setColumnWidth(Column::Title, 300);
    ui->tvTracks->setColumnWidth(Column::Artist, 250);
    ui->tvTracks->setColumnWidth(Column::Duration, 100);
    ui->tvTracks->setColumnWidth(Column::AudioType, 150);
    ui->tvTracks->setColumnWidth(Column::AudioFile, 250);
}

void MainWindow::folder_clicked(const QModelIndex& index)
{
    int folder_id = index.data(Qt::UserRole).toInt();
    auto audio = std::make_unique<AUDIO::Audio>();
    auto folder_filter = std::make_tuple(audio->folder()->dbColumnName(), " = ", folder_id);
    std::string filter_str = m_audio_entity_data_model->prepareFilter(folder_filter);
    //fetch_audio(filter_str);
    fetch_folder_audio(folder_id);
}


void MainWindow::artist_selected(const QModelIndex& index)
{
    auto first_name_column = ui->tvArtists->model()->index(index.row(), ArtistColumn::FirstName);
    auto artist_id = first_name_column.data(Qt::UserRole).toInt();
    auto audio = std::make_unique<AUDIO::Audio>();
    std::string filter_str = make_selection(m_artist_entity_data_model.get(), audio->artist()->dbColumnName(), artist_id);
    fetch_audio(filter_str);
}

void MainWindow::genre_selected(const QModelIndex& index)
{
    auto genre_column = ui->tvGenre->model()->index(index.row(), GenreColumn::GenreName);
    auto genre_id = genre_column.data(Qt::UserRole).toInt();
    auto audio = std::make_unique<AUDIO::Audio>();
    std::string filter_str = make_selection(m_genre_entity_data_model.get(), audio->genre()->dbColumnName(), genre_id);
    fetch_audio(filter_str);
}

std::string MainWindow::make_selection(EntityDataModel* edm, std::string column_name, int id)
{
    auto filter = std::make_tuple(column_name, " = ", id);
    std::string filter_str = edm->prepareFilter(filter);
    return filter_str;
    //fetch_audio(filter_str);
}
void MainWindow::fetch_audio(const std::string filter)
{
    m_audio_entity_data_model->clearEntities();
    m_audio_entity_data_model->search(filter);
    show_audio_data();
}

void MainWindow::fetch_folder_audio(int folder_id)
{
    m_audio_entity_data_model->clearEntities();

    AUDIO::Artist artist;
    AUDIO::Folder folder;
    AUDIO::Genre genre;

    auto const& audio = dynamic_cast<AUDIO::Audio*>(m_audio_entity_data_model->get_entity().get());

    auto folder_filter = std::make_tuple(audio->folder()->qualified_column_name<AUDIO::Audio>(),"=", folder_id);
    FRAMEWORK::RelationMapper* r_mapper = m_audio_entity_data_model->select_related(folder, artist, genre)->filter(folder_filter);

    try{
        m_audio_entity_data_model->readRaw(r_mapper->query());
    } catch(DatabaseException& de) {
        showMessage(de.errorMessage()); // write to log file
    }

    r_mapper->map_data();

    for (auto const& [record_id, record] : r_mapper->mapped_entities()){

        auto audio_uPtr = std::make_unique<AUDIO::Audio>();

        bool audio_is_constructed = false;

        for (auto const& [table_name, entities] : record){

            for (auto const& entity : entities){

                if (audio_uPtr->tableName() == entity->tableName() &&
                    !audio_is_constructed){
                    if (entity->id() > -1 ){
                        auto audio_ptr = dynamic_cast<AUDIO::Audio*>(entity.get());
                        *audio_uPtr.get() = *audio_ptr;
                        audio_is_constructed = true;
                        break;
                    }
                }

                auto const& artist = audio_uPtr->artist()->data_model_entity();
                if (artist == nullptr){
                    continue;
                }

                if (artist->tableName() == entity->tableName()){
                    auto artist_ptr = dynamic_cast<AUDIO::Artist*>(entity.get());

                    auto artist_uptr = std::make_unique<AUDIO::Artist>();
                    *artist_uptr.get() = *artist_ptr;
                    audio->artist()->set_fk_entity(std::move(artist_uptr));
                }

                auto const& folder = audio_uPtr->folder()->data_model_entity();
                if (folder == nullptr){
                    continue;
                }

                if (folder->tableName() == entity->tableName()){
                    auto folder_ptr = dynamic_cast<AUDIO::Folder*>(entity.get());
                    auto folder_uptr = std::make_unique<AUDIO::Folder>();

                    *folder_uptr.get() = *folder_ptr;
                    audio->folder()->set_fk_entity(std::move(folder_uptr));
                }

            }

        }

        if (audio_uPtr->audio_type()->value() != ""){
            m_audio_entity_data_model->add_entity(std::move(audio_uPtr));
        }

    }

    show_audio_data();

}

void MainWindow::show_audio_data()
{
    set_track_view();

    for(auto& [name, entity] : m_audio_entity_data_model->modelEntities()){
        AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(entity.get());

        if (audio->audio_type()->displayName() == "Song")
            m_audio_lib_item->create_row_item<AUDIO::SongAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Jingle")
            m_audio_lib_item->create_row_item<AUDIO::JingleAudioLibItem>(audio);

        if (audio->audio_type()->displayName() == "Commercial")
            m_audio_lib_item->create_row_item<AUDIO::CommercialAudioLibItem>(audio);
    }

}

void MainWindow::fetch_artist(const std::string filter)
{
    if (filter.empty()){
        m_artist_entity_data_model->all();
    }else{
        m_artist_entity_data_model->search(filter);
    }

    show_artist_data();
}

void MainWindow::show_artist_data()
{
    m_artist_model->clear();
    setup_artists_view();

    for(auto& [name, entity] : m_artist_entity_data_model->modelEntities()){
        AUDIO::Artist* artist = dynamic_cast<AUDIO::Artist*>(entity.get());

        if (artist->artist_type()->displayName() == "Female")
            m_artist_type_item->create_row_item<AUDIO::FemaleArtistTypeItem>(artist);

        if (artist->artist_type()->displayName() == "Male")
            m_artist_type_item->create_row_item<AUDIO::MaleArtistTypeItem>(artist);

        if (artist->artist_type()->displayName() == "Group")
            m_artist_type_item->create_row_item<AUDIO::GroupArtistTypeItem>(artist);
    }

}

void MainWindow::add_artist()
{
    std::unique_ptr<AUDIO::Artist> artist = std::make_unique<AUDIO::Artist>();
    std::unique_ptr<ArtistForm> art_form = std::make_unique<ArtistForm>(artist.get());
    if (art_form->exec() > 0){
        m_artist_entity_data_model->createEntity(std::move(artist));
        show_artist_data();
    }
}

void MainWindow::edit_artist()
{
    auto search_value = get_search_value(ui->tvArtists, ArtistColumn::FullName);
    std::unique_ptr<ArtistForm> dlg(nullptr);
    if (!search_value.empty()){
        BaseEntity* be = m_artist_entity_data_model->findEntityByName(search_value);
        if (be != nullptr){
            AUDIO::Artist* entity = dynamic_cast<AUDIO::Artist*>(be);
            dlg = std::make_unique<ArtistForm>(entity);
            if(dlg->exec() > 0){
                try{
                    update_table_view_record(ui->tvArtists, entity->tableViewValues());
                    m_artist_entity_data_model->updateEntity(*entity);
                    }
                    catch(DatabaseException& de){
                        showMessage(de.errorMessage());
                    }
                }else{
                    dlg = nullptr;
                }
            }
        }
}


void MainWindow::delete_artist()
{
    delete_entity(m_artist_entity_data_model.get(), ui->tvArtists, ArtistColumn::FullName);
}


void MainWindow::setup_genre_view()
{
    create_genre_view_headers();
    set_genre_view_columns_width();
}

void MainWindow::add_genre()
{
    std::unique_ptr<AUDIO::Genre> genre = std::make_unique<AUDIO::Genre>();
    std::unique_ptr<GenreForm> gen_form = std::make_unique<GenreForm>(genre.get());
    if (gen_form->exec() > 0){
        m_genre_entity_data_model->createEntity(std::move(genre));
        show_genre_data();
    }
}

void MainWindow::edit_genre()
{
    auto search_value = get_search_value(ui->tvGenre, GenreColumn::GenreName);
    if (search_value.empty())
        return;

    BaseEntity* be = m_genre_entity_data_model->findEntityByName(search_value);
    if (be == nullptr)
        return;

    std::unique_ptr<GenreForm> dlg(nullptr);
    AUDIO::Genre* entity = dynamic_cast<AUDIO::Genre*>(be);
    dlg = std::make_unique<GenreForm>(entity);
    if(dlg->exec() > 0){
        try{
            update_table_view_record(ui->tvGenre, entity->tableViewValues());
            m_genre_entity_data_model->updateEntity(*entity);
            }
            catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }else{
            dlg = nullptr;
        }


}


void MainWindow::delete_genre()
{
    delete_entity(m_genre_entity_data_model.get(), ui->tvGenre, GenreColumn::GenreName);
}



void MainWindow::create_genre_view_headers()
{
    m_genre_model->setHorizontalHeaderItem(0, new QStandardItem("Genre Name"));
}

void MainWindow::set_genre_view_columns_width()
{
    enum Column{GenreName};
    ui->tvGenre->setColumnWidth(Column::GenreName, 200);
}

void MainWindow::fetch_genre(const std::string filter)
{
    if (filter.empty()){
        m_genre_entity_data_model->all();
    }else{
        m_genre_entity_data_model->search(filter);
    }

    show_genre_data();
}

void MainWindow::show_genre_data()
{
    m_genre_model->clear();
    setup_genre_view();

    for(auto& [name, entity] : m_genre_entity_data_model->modelEntities()){
        AUDIO::Genre* genre = dynamic_cast<AUDIO::Genre*>(entity.get());
        m_genre_type_item->create_row_item<AUDIO::GeneralGenreTypeItem>(genre);
    }
}

void MainWindow::delete_entity(EntityDataModel* edm, QTableView* tv, int col)
{
    if (selected_row_id(tv) < 0)
        return;

    BaseEntity* entity = find_selected_entity(edm, tv, col);

    if (okay_to_delete(entity)){

        entity->setDBAction(DBAction::dbaDELETE);

        try{
            edm->deleteEntity(*entity);
            remove_tv_selected_row(tv);
        } catch (DatabaseException& de){
            showMessage(de.errorMessage());
        }
    }
}

bool MainWindow::okay_to_delete(const BaseEntity* entity)
{
    QMessageBox::StandardButton result_btn = QMessageBox::Yes;
    result_btn = QMessageBox::question(this, "Sedric",
                          tr("Are you sure you want to delete?"),
                          QMessageBox::No | QMessageBox::Yes,
                          QMessageBox::No);
    if (result_btn == QMessageBox::Yes)
    {
        return true;
    }

    return false;
}

void MainWindow::remove_tv_selected_row(QTableView* tv)
{
    tv->model()->removeRow(selected_row_id(tv));
}

void MainWindow::track_double_clicked(const QModelIndex& /*index*/)
{
    audio_properties();
}

std::string MainWindow::get_search_value(const QTableView* tv, int col)
{
    QItemSelectionModel* select = tv->selectionModel();
    auto mod_index = tv->model()->index(select->currentIndex().row(), col);
    auto col_value = tv->model()->data(mod_index, col).toString();
    std::string search_value = col_value.toStdString();
    return search_value;
}

BaseEntity* MainWindow::find_selected_entity(EntityDataModel* edm, QTableView* tv, int col)
{
    std::string search_value = get_search_value(tv, col);
    BaseEntity* entity = edm->findEntityByName(search_value);
    return entity;
}

void MainWindow::update_table_view_record(QTableView* tv,
        const std::vector<std::string> values)
{
    int col = 0;
    for (auto value : values){
        QVariant qvalue(QString::fromStdString(value));
        QModelIndex model_index = tv->model()->index(selected_row_id(tv), col);
        QAbstractItemModel* model = tv->model();
        model->setData(model_index, qvalue);
        ++col;
    }
}

int MainWindow::selected_row_id(QTableView* tv)
{
    if (tv->selectionModel()->selectedRows().size() > 0)
        return tv->selectionModel()->selectedRows()[0].row();
    return -1;
}

void MainWindow::search_artist()
{
    if (ui->edtSearchArtist->text().isEmpty()){
        fetch_artist();
        return;
    }

    std::string search_value = ui->edtSearchArtist->text().toStdString();
    auto artist = std::make_unique<AUDIO::Artist>();
    auto artist_filter = std::make_tuple(artist->fullName()->dbColumnName(), "like", search_value);
    std::string filter_str = m_artist_entity_data_model->prepareFilter(artist_filter);
    fetch_artist(filter_str);
}

void MainWindow::audio_properties()
{
    auto search_value = get_search_value(ui->tvTracks, TrackColumns::Title);
    if (search_value.empty())
        return;

    BaseEntity* be = m_audio_entity_data_model->findEntityByName(search_value);
    if (be == nullptr)
        return;

    AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(be);

    std::unique_ptr<AudioForm> audio_form = std::make_unique<AudioForm>(audio);
    if (audio_form->exec() > 0){
        try{
                update_table_view_record(ui->tvTracks, audio->tableViewValues());
                m_audio_entity_data_model->updateEntity(*audio);
            }
            catch(DatabaseException& de){
                qDebug() << QString::fromStdString(de.errorMessage());
                showMessage(de.errorMessage());
            }
    }

}
