#include <filesystem>
#include <algorithm>
#include <format>

#include <QDebug>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "datatoolbar.h"
#include "artistform.h"
#include "audioform.h"
#include "foldernameform.h"
#include "trashcanform.h"
#include "setupform.h"
#include "bookedaudiocheckerform.h"

#include "../audio/audiolibrary.h"
#include "../audio/audiolibitem.h"
#include "../audio/artisttypeitem.h"
#include "../audio/genretypeitem.h"
#include "../audio/audiotool.h"
#include "../audio/genreform.h"
#include "../audio/audioconverter.h"
#include "../audio/mp3oggconverter.h"
#include "../audio/oggtooggconverter.h"
#include "../audio/mtstomp3converter.h"
#include "../audio/audiowaveformgenerator.h"
#include "../audio/audiofileinfo.h"

#include "../audio/editor/audiowaveform.h"
#include "../audio/editor/audioplayer.h"

#include "../framework/baseentity.h"
#include "../framework/treeviewmodel.h"
#include "../framework/ravenexception.h"
#include "../framework/choicefield.h"
#include "../framework/letterfilterwidget.h"
#include "../framework/relationmapper.h"
#include "../framework/ravensetup.h"

namespace fs = std::filesystem;


MainWindow::MainWindow(QApplication* qapp, QWidget *parent)
    :QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,m_qapp{qapp}
    ,m_letter_filter_widget{nullptr}
    ,m_cut_folder_id{-1}
{
    ui->setupUi(this);

    // Menubar
    connect(ui->actTrashCan, &QAction::triggered, this, &MainWindow::open_trash_can);
    connect(ui->actSettings, &QAction::triggered, this, &MainWindow::open_settings);

    m_artist_toolbar = new DataToolBar();
    ui->hlArtist->addWidget(m_artist_toolbar);

    // Audio Tab
    connect(ui->btnSearchAudio, &QPushButton::clicked, this, &MainWindow::search_audio);
    connect(ui->edtSearchAudio, &QLineEdit::returnPressed, this, &MainWindow::search_audio);

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
    connect(ui->btnImport, &QPushButton::clicked, this, &MainWindow::import_audio);
    connect(ui->btnAudioProp, &QPushButton::clicked, this, &MainWindow::audio_properties);
    connect(ui->btnPlay, &QPushButton::clicked, this, &MainWindow::play_btn_clicked);
    connect(ui->btnCueEdit, &QPushButton::clicked, this, &MainWindow::cue_edit);
    connect(ui->btnEditor, &QPushButton::clicked, this, &MainWindow::audio_editor);
    connect(ui->btnCut, &QPushButton::clicked, this, &MainWindow::cut_audio);
    connect(ui->btnPaste, &QPushButton::clicked, this, &MainWindow::paste_audio);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::delete_audio);

    track_toolbar_icons();

    m_audio_edm = std::make_unique<EntityDataModel>(std::make_shared<AUDIO::Audio>());
    m_audio_entity_data_model = std::make_unique<EntityDataModel>(std::make_shared<AUDIO::Audio>());

    m_genre_entity_data_model = std::make_unique<EntityDataModel>(std::make_shared<AUDIO::Genre>());

    setup_audio_folders();

    m_tracks_model = new QStandardItemModel(this);
    create_track_view_headers();
    set_track_view_column_width();

    m_audio_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_tracks_model);

    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    connect(ui->tvTracks, &QTableView::doubleClicked, this, &MainWindow::track_double_clicked);
    ui->tvTracks->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvTracks, &QTableView::customContextMenuRequested, this, &MainWindow::track_context_menu);

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

    show_letter_filter();

    m_setup_edm = std::make_unique<EntityDataModel>(std::make_shared<RavenSetup>());
    m_setup_edm->all();
    m_setup = dynamic_cast<RavenSetup*>(m_setup_edm->firstEntity().get());

    m_editor_process = std::make_unique<QProcess>(this);

    ui->tabWidget->setCurrentIndex(0);
    setWindowTitle("Raven - Audio Explorer");

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_artist_toolbar;
    delete m_genre_toolbar;
    delete m_tracks_model;
    delete m_artist_model;
    delete m_genre_model;
    delete m_folder_model;
}

void MainWindow::attach_folder_model()
{
    //AudioLibrary audio_lib;
    //auto tree_data = audio_lib.read_data_from_db();
    //m_folder_model = new TreeViewModel(tree_data, this);
    m_folder_model->rebuild_tree();
    ui->tvFolders->setModel(m_folder_model);
    m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));
}

void MainWindow::setup_audio_folders()
{
    try{

        AudioLibrary audio_lib;
        auto tree_data = audio_lib.read_data_from_db();
        m_folder_model = new TreeViewModel(tree_data, this);
        ui->tvFolders->setModel(m_folder_model);
        m_folders = tree_data;

        m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));
        //attach_folder_model();

        connect(ui->tvFolders, &QTreeView::clicked, this, &MainWindow::folder_clicked);

        ui->tvFolders->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->tvFolders, &QTreeView::customContextMenuRequested, this, &MainWindow::folder_context_menu);

        connect_toolbutton_signals();

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
    m_tracks_model->setHorizontalHeaderItem(5, new QStandardItem("Folder"));
    m_tracks_model->setHorizontalHeaderItem(6, new QStandardItem("File Extension"));
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
    m_audio_lib_item->clear();

    create_track_view_headers();
    set_track_view_column_width();
}

void MainWindow::set_track_view_column_width()
{
    enum Column{Title, Artist, Duration, AudioType, AudioFile, Folder, FileExt};
    ui->tvTracks->setColumnWidth(Column::Title, 300);
    ui->tvTracks->setColumnWidth(Column::Artist, 250);
    ui->tvTracks->setColumnWidth(Column::Duration, 100);
    ui->tvTracks->setColumnWidth(Column::AudioType, 150);
    ui->tvTracks->setColumnWidth(Column::AudioFile, 250);
    ui->tvTracks->setColumnWidth(Column::Folder, 250);
    ui->tvTracks->setColumnWidth(Column::FileExt, 150);
}

void MainWindow::folder_clicked(const QModelIndex& index)
{
    int folder_id = selected_folder_id();
    auto audio = std::make_unique<AUDIO::Audio>();
    auto folder_filter = std::make_tuple(audio->folder()->qualified_column_name<AUDIO::Audio>(),"=", folder_id);
    //fetch_folder_audio(folder_filter);
    fetch_filtered_audio(folder_filter);
}

void MainWindow::track_toolbar_icons()
{
    QSize btn_icon_size(50, 50);

    ui->btnImport->setIconSize(btn_icon_size);
    ui->btnImport->setIcon(QIcon(":/images/media/icons/import_audio_sm.png"));
    ui->btnImport->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnAudioProp->setIconSize(btn_icon_size);
    ui->btnAudioProp->setIcon(QIcon(":/images/media/icons/audio_prop_sm.png"));
    ui->btnAudioProp->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnPlay->setIconSize(btn_icon_size);
    ui->btnPlay->setIcon(QIcon(":/images/media/icons/listen_sm.png"));
    ui->btnPlay->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnCueEdit->setIconSize(btn_icon_size);
    ui->btnCueEdit->setIcon(QIcon(":/images/media/icons/cue_edit_sm.png"));
    ui->btnCueEdit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnEditor->setIconSize(btn_icon_size);
    ui->btnEditor->setIcon(QIcon(":/images/media/icons/audacity_sm.png"));
    ui->btnEditor->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnCut->setIconSize(btn_icon_size);
    ui->btnCut->setIcon(QIcon(":/images/media/icons/cut_document_sm.png"));
    ui->btnCut->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnPaste->setIconSize(btn_icon_size);
    ui->btnPaste->setIcon(QIcon(":/images/media/icons/paste_sm.png"));
    ui->btnPaste->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnDelete->setIconSize(btn_icon_size);
    ui->btnDelete->setIcon(QIcon(":/images/media/icons/delete_sm.png"));
    ui->btnDelete->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->btnSearchAudio->setIconSize(QSize(30,30));
    ui->btnSearchAudio->setIcon(QIcon(":/images/media/icons/search_audio.bmp"));
    ui->btnSearchAudio->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);



}

void MainWindow::track_context_menu(const QPoint& pos)
{
    QMenu context_menu("Context Menu", ui->tvTracks);

    QAction actPlay("Play audio", ui->tvTracks);
    connect(&actPlay, &QAction::triggered, this, &MainWindow::play_audio);

    QAction actStop("Stop play", ui->tvTracks);
    connect(&actPlay, &QAction::triggered, this, &MainWindow::stop_play);

    QAction actCueEdit("Cue edit...", ui->tvTracks);
    connect(&actCueEdit, &QAction::triggered, this, &MainWindow::cue_edit);

    QAction actEditor("Audio editor...", ui->tvTracks);
    connect(&actEditor, &QAction::triggered, this, &MainWindow::audio_editor);

    QAction actCutAudio("Cut audio", ui->tvTracks);
    connect(&actCutAudio, &QAction::triggered, this, &MainWindow::cut_audio);

    QAction actPasteAudio("Paste Audio", ui->tvTracks);
    connect(&actPasteAudio, &QAction::triggered, this, &MainWindow::paste_audio);

    QAction actProp("Audio properties...", ui->tvTracks);
    connect(&actProp, &QAction::triggered, this, &MainWindow::audio_properties);

    context_menu.addAction(&actPlay);
    context_menu.addAction(&actStop);
    context_menu.addSeparator();
    context_menu.addAction(&actCueEdit);
    context_menu.addAction(&actEditor);
    context_menu.addSeparator();
    context_menu.addAction(&actCutAudio);
    context_menu.addAction(&actPasteAudio);
    context_menu.addSeparator();
    context_menu.addAction(&actProp);

    context_menu.exec(ui->tvTracks->mapToGlobal(pos));
}

void MainWindow::folder_context_menu(const QPoint& pos)
{
    QMenu context_menu("Context Menu", ui->tvFolders);

    QAction actNew("New Folder", ui->tvFolders);
    connect(&actNew, &QAction::triggered, this, &MainWindow::create_new_folder);
    actNew.setIcon(QIcon(":/images/media/icons/add.png"));

    QAction actRename("Rename Folder", ui->tvFolders);
    connect(&actRename, &QAction::triggered, this, &MainWindow::rename_folder);
    actRename.setIcon(QIcon(":/images/media/icons/edit.png"));

    QAction actCut("Cut Folder", ui->tvFolders);
    connect(&actCut, &QAction::triggered, this, &MainWindow::cut_folder);
    actCut.setIcon(QIcon(":/images/media/icons/cut.png"));

    QAction actPaste("Paste Folder", ui->tvFolders);
    connect(&actPaste, &QAction::triggered, this, &MainWindow::paste_folder);
    actPaste.setIcon(QIcon(":/images/media/icons/paste.png"));

    QAction actDelete("Delete Folder", ui->tvFolders);
    connect(&actDelete, &QAction::triggered, this, &MainWindow::delete_folder);
    actDelete.setIcon(QIcon(":/images/media/icons/delete.png"));

    context_menu.addAction(&actNew);
    context_menu.addAction(&actRename);
    context_menu.addSeparator();
    context_menu.addAction(&actCut);
    context_menu.addAction(&actPaste);
    context_menu.addSeparator();
    context_menu.addAction(&actDelete);
    context_menu.exec(ui->tvFolders->mapToGlobal(pos));

}

void MainWindow::connect_toolbutton_signals()
{
    connect(ui->tbNew, &QToolButton::clicked, this, &MainWindow::create_new_folder);
    ui->tbNew->setIconSize(QSize(40,40));
    ui->tbNew->setIcon(QIcon(":/images/media/icons/new_folder2_sm.bmp"));
    ui->tbNew->setToolTip("New Folder");


    connect(ui->tbRename, &QToolButton::clicked, this, &MainWindow::rename_folder);
    ui->tbRename->setIconSize(QSize(40,40));
    ui->tbRename->setIcon(QIcon(":/images/media/icons/edit_folder2_sm.bmp"));
    ui->tbRename->setToolTip("Rename Selected Folder");

    connect(ui->tbCut, &QToolButton::clicked, this, &MainWindow::cut_folder);
    ui->tbCut->setIconSize(QSize(40,40));
    ui->tbCut->setIcon(QIcon(":/images/media/icons/cut.png"));
    ui->tbCut->setToolTip("Cut Selected Folder");

    connect(ui->tbPaste, &QToolButton::clicked, this, &MainWindow::paste_folder);
    ui->tbPaste->setIconSize(QSize(40,40));
    ui->tbPaste->setIcon(QIcon(":/images/media/icons/paste_folder_sm.bmp"));
    ui->tbPaste->setToolTip("Paste Cut Folder");

    connect(ui->tbDelete, &QToolButton::clicked, this, &MainWindow::delete_folder);
    ui->tbDelete->setIconSize(QSize(40,40));
    ui->tbDelete->setIcon(QIcon(":/images/media/icons/delete_folder_sm.bmp"));
    ui->tbDelete->setToolTip("Delete Selected Folder");
}


int MainWindow::selected_folder_id()
{
   auto selected_indexes = ui->tvFolders->selectionModel()->selectedIndexes();
    int id = 0;
    if (selected_indexes.size() > 0 ){
        auto selected_index = selected_indexes[0];
        id = selected_index.data(Qt::UserRole).toInt();
    }
    return id;
}

int MainWindow::selected_folder_row()
{
   auto selected_indexes = ui->tvFolders->selectionModel()->selectedIndexes();
    int row = 0;
    if (selected_indexes.size() > 0 ){
        auto selected_index = selected_indexes[0];
        row = selected_index.row();
    }
    return row;

}

void MainWindow::create_new_folder()
{
    auto folder_name_form = std::make_unique<FolderNameForm>("",this);

    if (folder_name_form->exec() > 0)
    {
        std::string folder_name = folder_name_form->folder_name().toStdString();

        int id;
        int folder_id = selected_folder_id();
        try{
            id = create_folder_to_db(folder_name, folder_id);
        }catch(DatabaseException& de){
            qDebug() << QString::fromStdString(de.errorMessage());
        }

        ui->tvFolders->setModel(nullptr);

        NodeData* nd = new NodeData();
        nd->id = id;
        nd->parent_id = folder_id;
        nd->name = folder_name;
        m_folders.push_back(nd);

        delete m_folder_model;
        m_folder_model = new TreeViewModel(m_folders, this);
        ui->tvFolders->setModel(m_folder_model);
        m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));

        auto current_row = selected_folder_row();
        QModelIndex index = m_folder_model->index(current_row, 0, QModelIndex());

        ui->tvFolders->setCurrentIndex(index);
        ui->tvFolders->expand(index);
    }
}

int MainWindow::create_folder_to_db(const std::string& folder_name, int parent_id)
{
    std::stringstream sql;
    sql << "Insert into rave_folder (folder_name, parent) Values ('"+folder_name+"'"
        << ","+std::to_string(parent_id)+") RETURNING id";

    EntityDataModel edm;
    return edm.insert_returning_id(sql.str());
}

void MainWindow::rename_folder()
{
    int folder_id = ui->tvFolders->selectionModel()->currentIndex().data(Qt::UserRole).toInt();
    QString folder_name =  m_folder_model->itemFromIndex(ui->tvFolders->currentIndex())->text();
    auto folder_name_form = std::make_unique<FolderNameForm>(folder_name, this);

    if (folder_name_form->exec() > 0){
        QString new_name = folder_name_form->folder_name();
        try{
            update_folder_name_db(folder_id, new_name.toStdString());
            QStandardItem* item = m_folder_model->itemFromIndex(ui->tvFolders->currentIndex());
            item->setText(new_name);
        } catch(DatabaseException& de) {
            showMessage(de.errorMessage());
        }
    }
}

void MainWindow::cut_folder()
{
    m_cut_folder_id = ui->tvFolders->selectionModel()->currentIndex().data(Qt::UserRole).toInt();
}

void MainWindow::paste_folder()
{
    int new_parent_id = ui->tvFolders->selectionModel()->currentIndex().data(Qt::UserRole).toInt();

    try{
        update_folder_parent(m_cut_folder_id, new_parent_id);
        update_folder_view(new_parent_id);
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }
}


void MainWindow::update_folder_view(int new_parent_id)
{
    for (auto folder : m_folders){
        if (folder->id == m_cut_folder_id){
            folder->parent_id = new_parent_id;
            break;
        }
    }

    auto current_row = ui->tvFolders->selectionModel()->currentIndex().row();
    ui->tvFolders->setModel(nullptr);
    delete m_folder_model;

//    m_folder_model = new TreeViewModel(m_folders, this);
    ui->tvFolders->setModel(m_folder_model);
    m_folder_model->setHorizontalHeaderItem(0, new QStandardItem("Audio Library"));

    QModelIndex index = m_folder_model->index(current_row, 0, QModelIndex());
    ui->tvFolders->setCurrentIndex(index);
    ui->tvFolders->expand(index);

}

void MainWindow::update_folder_name_db(int folder_id, std::string new_name)
{
    std::stringstream sql;
    sql << "Update rave_folder set folder_name = '"+new_name+"'"
        << " Where id = "+std::to_string(folder_id);
    EntityDataModel edm;
    edm.executeRawSQL(sql.str());
}

void MainWindow::update_folder_parent(int folder_id, int new_parent_id)
{
    std::stringstream sql;
    sql << "Update rave_folder set parent = "+std::to_string(new_parent_id)
        << " Where id = "+std::to_string(folder_id);
    EntityDataModel edm;
    edm.executeRawSQL(sql.str());
}


void MainWindow::delete_folder()
{
    // if folder is empty, delete.
    int folder_id = ui->tvFolders->selectionModel()->currentIndex().data(Qt::UserRole).toInt();

    if (!is_folder_empty(folder_id)){
        showMessage("Cannot delete a folder with content!");
        return;
    }

    try{
        delete_folder_db(folder_id);
        auto mod_index = ui->tvFolders->selectionModel()->currentIndex();
        ui->tvFolders->model()->removeRow(mod_index.row(), mod_index.parent());
    }catch(DatabaseException& de) {
        showMessage("DB Error: "+de.errorMessage());
    }
}

bool MainWindow::is_folder_empty(int folder_id)
{
    auto audio = std::make_unique<AUDIO::Audio>();
    std::string column_name = audio->folder()->dbColumnName();
    std::unique_ptr<EntityDataModel> edm = std::make_unique<EntityDataModel>(std::move(audio));
    auto audio_filter = std::make_tuple(column_name, " = ", folder_id);
    auto filter = edm->prepareFilter(audio_filter);

    qDebug() << stoq(filter);

    edm->search(filter);

    return (edm->count() == 0) ? true : false;

}

void MainWindow::delete_folder_db(int folder_id)
{
    std::stringstream sql;
    sql << "Delete from rave_folder where id = "+std::to_string(folder_id);
    EntityDataModel edm;
    edm.executeRawSQL(sql.str());
}


void MainWindow::artist_selected(const QModelIndex& index)
{
    auto first_name_column = ui->tvArtists->model()->index(index.row(), ArtistColumn::FirstName);
    auto artist_id = first_name_column.data(Qt::UserRole).toInt();
    auto audio = std::make_unique<AUDIO::Audio>();
    //std::string filter_str = make_selection(m_artist_entity_data_model.get(), audio->artist()->dbColumnName(), artist_id);
    std::string filter_str = make_selection(m_artist_manager.model().get(), audio->artist()->dbColumnName(), artist_id);
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
}
void MainWindow::fetch_audio(const std::string filter)
{
    m_audio_entity_data_model->clearEntities();
    m_audio_entity_data_model->search(filter);
    show_audio_data();
}


void MainWindow::fetch_folder_audio(FRAMEWORK::RelationMapper* r_mapper)
{
    m_audio_entity_data_model->clearEntities();

    auto audio = dynamic_cast<AUDIO::Audio*>(m_audio_entity_data_model->get_entity().get());

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

//*-------

void MainWindow::fetch_artist(const std::string filter)
{
    m_artist_manager.fetch_data(filter);
    show_artist_data();
}

void MainWindow::show_artist_data()
{
    m_artist_model->clear();
    setup_artists_view();

    for(auto& [name, entity] : m_artist_manager.model()->modelEntities()){
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
        m_artist_manager.create(std::move(artist));
        show_artist_data();
    }
}

void MainWindow::edit_artist()
{
    auto search_value = get_search_value(ui->tvArtists, ArtistColumn::FullName);
    auto artist = m_artist_manager.find_artist(search_value);
    if (artist == nullptr)
        return;

    std::unique_ptr<ArtistForm> dlg = std::make_unique<ArtistForm>(artist);
    if(dlg->exec() > 0){
        try{
            update_table_view_record(ui->tvArtists, artist->tableViewValues());
            m_artist_manager.update(artist);
            }
            catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
        }else{
            dlg = nullptr;
        }

}

void MainWindow::delete_artist()
{
    std::string search_value = get_search_value(ui->tvArtists, ArtistColumn::FullName);
    AUDIO::Artist* artist = m_artist_manager.find_artist(search_value);
    if (artist == nullptr)
        return;

    if (m_artist_manager.artist_has_audio(artist)){
        showMessage("Arist is attached to existing audio!");
        return;
     }

    try{
        m_artist_manager.delete_artist(artist);
        remove_tv_selected_row(ui->tvArtists);
    }catch (DatabaseException& de) {
        showMessage("Error deleting!: "+de.errorMessage());
    }

}


//*-------

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

    std::shared_ptr<BaseEntity> be = m_genre_entity_data_model->findEntityByName(search_value);
    if (be == nullptr)
        return;

    std::unique_ptr<GenreForm> dlg(nullptr);
    AUDIO::Genre* entity = dynamic_cast<AUDIO::Genre*>(be.get());
    dlg = std::make_unique<GenreForm>(entity);
    if(dlg->exec() > 0){
        try{
            update_table_view_record(ui->tvGenre, entity->tableViewValues());
            m_genre_entity_data_model->updateEntity(*entity);
            m_genre_entity_data_model->all();
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

    std::shared_ptr<BaseEntity> entity = find_selected_entity(edm, tv, col);

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

bool MainWindow::okay_to_delete(const std::shared_ptr<BaseEntity> entity)
{
    QMessageBox::StandardButton result_btn = QMessageBox::Yes;
    result_btn = QMessageBox::question(this, "Audio Explorer",
                          tr("Are you sure you want to delete?"),
                          QMessageBox::No | QMessageBox::Yes,
                          QMessageBox::No);

    return (result_btn == QMessageBox::Yes) ? true : false;
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

std::shared_ptr<BaseEntity> MainWindow::find_selected_entity(EntityDataModel* edm, QTableView* tv, int col)
{
    std::string search_value = get_search_value(tv, col);
    std::shared_ptr<BaseEntity> entity = edm->findEntityByName(search_value);
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

void MainWindow::search_audio()
{
    if (ui->edtSearchAudio->text().isEmpty())
        return;
    auto audio = std::make_unique<AUDIO::Audio>();
    auto filter =  std::make_tuple(audio->title()->qualified_column_name<AUDIO::Audio>(),
                                   "like",
                                   ui->edtSearchAudio->text().toStdString());
    fetch_filtered_audio(filter);
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
    std::string filter_str = m_artist_manager.model()->prepareFilter(artist_filter);

    fetch_artist(filter_str);
}


void MainWindow::import_audio()
{

    int folder_id = selected_folder_id();
    if (folder_id == 0){
        showMessage("Please select folder for import");
        return;
    }

    auto audio_file = QFileDialog::getOpenFileName(this,
                                                   tr("Import Audio"), QDir::currentPath(),
                                                   tr("Audio Files (*.mp3 *.ogg *.mts)"));
    if (audio_file.isEmpty())
        return;

    auto audio = std::make_unique<AUDIO::Audio>(audio_file.toStdString());
    
    audio->set_folder(folder_id);
    audio->set_audio_lib_path(m_setup->audio_folder()->value());

    auto audio_form = std::make_unique<AudioForm>(audio.get(), m_setup);

    // open the audio property window
    if (audio_form->exec() > 0)
    {

        auto shared_audio = std::make_shared<AUDIO::Audio>(audio_file.toStdString());
        convert_audio(shared_audio);

        QFileInfo afi(audio_file);
        QString file_format = afi.suffix().toLower();

        // TODO: Check if we are allowed to use MP3 before converting it to OGG

        if (file_format == "mp3"){
            m_audio_converter = std::make_unique<AUDIO::Mp3ToOggConverter>(shared_audio);
            m_audio_converter->convert();
        }

        if (file_format == "ogg"){
            m_audio_converter = std::make_unique<AUDIO::OggToOggConverter>(shared_audio);
            m_audio_converter->convert();
        }

        if (file_format == "mts"){
            m_audio_converter = std::make_unique<AUDIO::MtsToMp3Converter>(shared_audio);
            m_audio_converter->convert();
            auto mp3_file = dynamic_cast<AUDIO::MtsToMp3Converter*>(m_audio_converter.get());
            audio_file = mp3_file->dest_mp3_filename();
            audio->audio_file().set_audio_filename(audio_file.toStdString());
        }

        //TODO: Resolve the file extension issue (Ogg, Mp3, or MTS)

        //audio->set_file_extension(file_format.toUpper().toStdString());
        audio->set_file_extension("OGG");

        //convert_audio(audio);
       /*
        auto wave_file = generate_audio_wave_file(audio);
        bool updated = update_wave_file(wave_file);
        if (updated){
            commit_data()
        }
        */

        m_wave_gen = std::make_unique<AUDIO::AudioWaveFormGenerator>(audio_file);
        m_wave_gen->generate();
        audio->audio_file().set_wave_file(m_wave_gen->wave_filename().toStdString());

        auto audio_file_info = audio->audio_file();
        auto wave_form = std::make_unique<AUDIO::AudioWaveForm>(audio_file_info);
        if (wave_form->exec() == 1)
        {
            audio->set_duration(audio_file_info.duration());
            audio->set_cue_marker(wave_form->cue_marker());
            audio->setDBAction(DBAction::dbaCREATE);

            audio_file_info.set_marker(wave_form->cue_marker());

            ImportResult import_result;

            int audio_id = write_audio_to_db(std::move(audio));

            import_result.audio_id = audio_id;

            if (audio_id == -1){
                rollback_import_process(import_result);
                audio->setDBAction(DBAction::dbaNONE);
                return;
            }

            auto [status, new_filename] = copy_new_audio_to_library(audio_id);
            import_result.temp_files.push_back(new_filename);
            if (!status) {
                rollback_import_process(import_result);
                audio->setDBAction(DBAction::dbaNONE);
                return;
            }

            auto [wave_file_is_created, wave_file] = copy_wave_file_to_library(audio_id);
            import_result.temp_files.push_back(wave_file);
            if (!wave_file_is_created){
                rollback_import_process(import_result);
                audio->setDBAction(DBAction::dbaNONE);
                return;
            }

            auto [adf_file_is_created, adf_file] = create_adf_file(audio_id, audio_file_info);
            import_result.temp_files.push_back(adf_file);
            if (!adf_file_is_created){
                rollback_import_process(import_result);
                audio->setDBAction(DBAction::dbaNONE);
                return;
            }

            m_audio_converter->remove_ogg_file();
            m_wave_gen->remove_wave_file();
         }

      }
}

bool MainWindow::convert_audio(std::shared_ptr<AUDIO::Audio> audio)
{
    auto file_ext = audio->file_extension()->value();

    if (file_ext == "mp3"){
        // TODO: Do we really need to convert MP3 to OGG, yet we can play MP3 audio just fine?
        m_audio_converter = std::make_unique<AUDIO::Mp3ToOggConverter>(audio);
        if (!m_audio_converter->convert()){
            //clean-up
            return false;
        }
    }

    if (file_ext == "ogg"){
        m_audio_converter = std::make_unique<AUDIO::OggToOggConverter>(audio);
        if (!m_audio_converter->convert()){
            // clean-up
            return false;
        }
    }

    if (file_ext == "mts"){
        m_audio_converter = std::make_unique<AUDIO::MtsToMp3Converter>(audio);
        m_audio_converter->convert();
        auto mp3_file = dynamic_cast<AUDIO::MtsToMp3Converter*>(m_audio_converter.get());
        auto audio_file = mp3_file->dest_mp3_filename();
        audio->audio_file().set_audio_filename(audio_file.toStdString());
    }

    return true;

}

void MainWindow::rollback_import_process(ImportResult&  import_result)
{
    for (auto& file : import_result.temp_files){
        //delete_temp_file(file);
    }
   // delete_audio_record_from_db(audio_id)
   m_audio_converter->remove_ogg_file();
   m_wave_gen->remove_wave_file();
}


int MainWindow::write_audio_to_db(std::unique_ptr<AUDIO::Audio> audio)
{
    int id = -1;

    try{
        id = m_audio_edm->createEntity(std::move(audio));
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
        return id;
    }

    return id;
}

FileOperationResult MainWindow::copy_new_audio_to_library(int audio_id)
{
    AUDIO::AudioTool at;
    auto audio_filename = at.make_audio_filename(audio_id);
    auto new_audio_file = m_setup->audio_folder()->value()+audio_filename+".ogg";
    auto src_audio_path = fs::path(m_audio_converter->ogg_filename().toStdString());
    auto dst_audio_path = fs::path(new_audio_file);

    try{
        fs::copy(src_audio_path, dst_audio_path);
    } catch (fs::filesystem_error& fe) {
        qDebug() << "Failed to copy new audio file to dest";
        return std::make_tuple(false, new_audio_file);
    }

    return std::make_tuple(true, new_audio_file);
}

FileOperationResult MainWindow::copy_wave_file_to_library(int audio_id)
{
    AUDIO::AudioTool at;
    auto audio_filename = at.make_audio_filename(audio_id);

    auto new_audio_wave_file = m_setup->audio_folder()->value()+audio_filename+".png";
    auto src_wave_path(m_wave_gen->wave_filename().toStdString());
    auto dst_wave_path(new_audio_wave_file);

    try{
        fs::copy(src_wave_path, dst_wave_path);
    } catch (fs::filesystem_error& fe) {
        qDebug() << "Failed to copy new wave file to dest";
        return std::make_tuple(false, new_audio_wave_file);
    }

    return std::make_tuple(true, new_audio_wave_file);
}

FileOperationResult MainWindow::create_adf_file(int audio_id, AudioFile af)
{
    AUDIO::AudioTool at;
    auto audio_filename = at.make_audio_filename(audio_id);

    auto adf_filename = m_setup->audio_folder()->value()+audio_filename+".adf";
    af.set_adf_file(adf_filename);
    af.set_audio_lib_path(m_setup->audio_folder()->value());
    af.set_ogg_short_filename(audio_filename+".ogg");

    AUDIO::ADFRepository adf_repo;
    return std::make_tuple(adf_repo.write(af), adf_filename);
}

void MainWindow::audio_properties()
{
    auto search_value = get_search_value(ui->tvTracks, TrackColumns::Title);
    if (search_value.empty())
        return;

    std::shared_ptr<BaseEntity> be = m_audio_entity_data_model->findEntityByName(search_value);
    if (be == nullptr)
        return;

    AUDIO::Audio* audio = dynamic_cast<AUDIO::Audio*>(be.get());

    audio->set_file_path(audio->audio_lib_path()->value());

    std::unique_ptr<AudioForm> audio_form = std::make_unique<AudioForm>(audio, m_setup);
    if (audio_form->exec() > 0){
        try{
                update_table_view_record(ui->tvTracks, audio->tableViewValues());
                m_audio_entity_data_model->updateEntity(*audio);
                //m_audio_entity_data_model->all();
            }
            catch(DatabaseException& de){
                showMessage(de.errorMessage());
            }
    }

}


void MainWindow::play_btn_clicked()
{
    if (selected_row_id(ui->tvTracks) < 0)
        return;

    if (!ui->btnPlay->isChecked()){
        stop_play();
        ui->btnPlay->setText("Play");
    } else {
        play_audio();
        ui->btnPlay->setText("Stop");
    }
}

void MainWindow::play_audio()
{
    /*
    QItemSelectionModel* select = ui->tvTracks->selectionModel();
    if (select->selectedRows().size() == 0)
        return;
    */

    qDebug() << "Play Audio";

    auto audio = get_selected_audio();
    if (audio == nullptr)
        return;

    qDebug() << "AAAA";

    std::string audio_format = audio->file_extension()->value();
    std::transform(audio_format.begin(), audio_format.end(), audio_format.begin(), [](unsigned char c){return std::tolower(c);});

    qDebug() << "BBBB";

    AUDIO::AudioTool at;
    auto ogg_file = at.make_audio_filename(audio->id())+"."+audio_format;
    auto full_audio_name = audio->audio_lib_path()->value()+ogg_file;

    qDebug() << "CCCC";

    if (!QFile::exists(QString::fromStdString(full_audio_name))){
        showMessage("File does not exists! "+full_audio_name);
        return;
    }

    qDebug() << "Full Audio Name: "<< stoq(full_audio_name);

    AudioFile af(full_audio_name);
    m_audio_player = std::make_unique<AUDIO::AudioPlayer>(af);
    connect(m_audio_player.get(), &AUDIO::AudioPlayer::end_of_play, this, &MainWindow::end_of_play);
    m_audio_player->play_audio("C", QString::fromStdString(full_audio_name));

//    m_cue_editor = std::make_unique<CueEditor>(af, m_qapp);
//    m_cue_editor->play_audio();
}

void MainWindow::stop_play()
{
    if (m_audio_player != nullptr)
        m_audio_player->stop_play();
}

void MainWindow::end_of_play()
{
    qDebug() << "Play ended ...";
    ui->btnPlay->setChecked(false);
    ui->btnPlay->setText("Play");
}

AUDIO::Audio* MainWindow::get_selected_audio()
{
    AUDIO::Audio* audio = nullptr;

    auto selected_indexes = ui->tvTracks->selectionModel()->selectedIndexes();
    if (selected_indexes.size() == 0)
        return audio;

    auto selected_index = selected_indexes[0];
    int audio_id = selected_index.data(Qt::UserRole).toInt();


    /*
    auto mod_index = ui->tvTracks->currentIndex();
    auto first_col = ui->tvTracks->model()->index(mod_index.row(), 0);
    auto audio_id = first_col.data(Qt::UserRole).toInt();
    if (audio_id == 0)
        return nullptr;
    */

    qDebug() << "Audio ID: "<< audio_id;

    audio = m_audio_lib_item->find_audio_by_id(audio_id);

    qDebug() << "4444";
    return audio;
}


void MainWindow::cue_edit()
{

    auto audio = get_selected_audio();
    if (audio == nullptr)
        return;

    std::string extension = audio->file_extension()->value();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c){return std::tolower(c);});

    AUDIO::AudioTool at;
    auto ogg_file = at.make_audio_filename(audio->id())+"."+extension;
    auto full_audio_name = audio->audio_lib_path()->value()+ogg_file;
    AudioFile aud_file(full_audio_name);

    aud_file.set_audio_title(audio->title()->value());
    aud_file.set_short_desc("");
    aud_file.set_artist_name(audio->artist_fullname());
    aud_file.set_audio_path(audio->audio_lib_path()->value());
    //aud_file.set_audio_file(ogg_file);
    aud_file.set_audio_type(audio->audio_type()->displayName());

    aud_file.set_duration(audio->duration()->value());

    aud_file.set_id(audio->id());
    aud_file.set_ogg_filename(full_audio_name);
    aud_file.set_ogg_short_filename(ogg_file);
    aud_file.set_audio_lib_path(audio->audio_lib_path()->value());

    audio->set_audio_file(aud_file);

    aud_file.set_marker(audio->cue_marker());

    if (!fs::exists(aud_file.wave_file())){
        auto wave_gen = std::make_unique<AUDIO::AudioWaveFormGenerator>(QString::fromStdString(aud_file.audio_file()));
        wave_gen->generate();
    }

//    auto cue_editor = std::make_unique<CueEditor>(aud_file, m_qapp);
    //CueEditor* cue_editor = new CueEditor(aud_file, m_qapp);
//    if (cue_editor->editor() == 1){

    auto wave_form = std::make_unique<AUDIO::AudioWaveForm>(aud_file);
    if (wave_form->exec()) {
        audio->set_audio_file(aud_file);
        audio->set_duration(audio->audio_file().duration());
    }

}


void MainWindow::audio_editor()
{
    auto audio = get_selected_audio();
    if (audio == nullptr)
        return;

    QString editor = m_setup->editor_filepath()->to_qstring();
    if (!QFile::exists(editor)) {
        showMessage("Audio editor missing! Editing aborted.");
        return;
    }

    if (!QFile::exists(audio->full_audio_filename())){
        showMessage("Missing audio file! Editing aborted.");
        return;
    }

    QStringList args;
    args << audio->full_audio_filename();

    m_editor_process->start(editor, args);
}

void MainWindow::cut_audio()
{
    if (selected_row_id(ui->tvTracks) < 0)
        return;

    m_cut_audios.clear();

    QItemSelectionModel* selection = ui->tvTracks->selectionModel();
    QModelIndexList rows = selection->selectedRows(0);

    for (auto model_index : rows){
        int audio_id = model_index.data(Qt::UserRole).toInt();
        m_cut_audios.push_back(audio_id);
    }

}

void MainWindow::paste_audio()
{
    if (ui->tvFolders->selectionModel()->selectedRows().size() == 0){
        showMessage("Please select a folder to paste audio");
        return;
    }

    if (m_cut_audios.size() == 0){
        showMessage("No audio to paste!");
        return;
    }

    try{
        move_audio_to_current_folder();
        m_cut_audios.clear();
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

}

void MainWindow::delete_audio()
{

    QModelIndexList selected_rows = ui->tvTracks->selectionModel()->selectedRows(0);
    if (selected_rows.count() == 0)
        return;

    std::map<int, std::tuple<std::string, bool>> selected_audios;
    std::map<int, QModelIndex> selected_indexes;

    for (auto model_index : selected_rows){
        int audio_id = model_index.data(Qt::UserRole).toInt();
        selected_audios[audio_id] = std::make_tuple(model_index.data().toString().toStdString(), true);
        selected_indexes[audio_id] = model_index;
    }

    auto bacf = std::make_unique<BookedAudioCheckerForm>(selected_audios);
    if (bacf->exec() == 0){
        return;
    }

    std::string ids;
    for (auto [id, data] : selected_audios){
        auto [title, ok_del] = data;
        if (ok_del){
            if (!ids.empty())
                ids += ",";
            ids += std::to_string(id);
        }
    }

    auto select_model = ui->tvTracks->selectionModel();

    for (auto [id, data] : selected_audios){
        auto [title, ok_del] = data;
        if (!ok_del){
            select_model->select(selected_indexes[id], QItemSelectionModel::Deselect);
        }
    }

    QModelIndexList rows = ui->tvTracks->selectionModel()->selectedRows(0);

    int row_count = rows.count();
    try{
        for (int i=row_count; i > 0; i--){
            auto text = rows.at(i-1).data().toString();
            std::shared_ptr<BaseEntity> audio = m_audio_entity_data_model->findEntityByName(text.toStdString());
            audio->setDBAction(DBAction::dbaDELETE);

            delete_audio_from_db(ids);
            m_audio_entity_data_model->deleteFromModel();

            ui->tvTracks->model()->removeRow(rows.at(i-1).row(), rows.at(i-1).parent());
        }
    } catch (DatabaseException& de) {
        showMessage(de.errorMessage());
    }

}

void MainWindow::delete_audio_from_db(const std::string ids)
{
    // Remove audio file from database

    std::stringstream sql;

    sql << "Update rave_audio set deleted = true "
        << " Where id in ("+ids+")";

    EntityDataModel edm;
    edm.executeRawSQL(sql.str());
}

void MainWindow::move_audio_to_current_folder()
{
    std::stringstream sql;
    int folder_id = ui->tvFolders->selectionModel()->currentIndex().data(Qt::UserRole).toInt();

    std::string ids;
    for (int id : m_cut_audios){
        if (!ids.empty())
            ids += ", ";
        ids += std::to_string(id);
    }

    sql << "Update rave_audio set folder_id="+std::to_string(folder_id)
        << " Where id in ("+ids+")";

    EntityDataModel edm;
    edm.executeRawSQL(sql.str());

    auto audio = std::make_unique<AUDIO::Audio>();
    auto folder_filter = std::make_tuple(audio->folder()->qualified_column_name<AUDIO::Audio>(),"=", folder_id);
    //fetch_folder_audio(folder_filter);
    fetch_filtered_audio(folder_filter);
}

void MainWindow::show_letter_filter()
{
    m_letter_filter_widget = std::make_unique<LetterFilterWidget>(this);
    ui->hlayLetterFilter->addWidget(m_letter_filter_widget.get());
    connect(m_letter_filter_widget->get_tabwidget(), &QTabWidget::currentChanged, this, &MainWindow::filter_audio_by_letter);
}

void MainWindow::filter_audio_by_letter(int index)
{
    QString tab_text = m_letter_filter_widget->get_tabwidget()->tabText(index);

    auto search = std::make_tuple("title", tab_text.toStdString());
    m_audio_entity_data_model->starts_with_view(search);

    show_audio_data();
}

void MainWindow::open_trash_can()
{
    auto trash_can = std::make_unique<TrashCanForm>(this);
    trash_can->exec();
}

void MainWindow::open_settings()
{
    EntityDataModel edm(std::make_shared<RavenSetup>());
    edm.all();

    if (edm.count() > 0)
    {
        std::shared_ptr<BaseEntity> be = edm.firstEntity();
        RavenSetup* rs = dynamic_cast<RavenSetup*>(be.get());

        auto sform = std::make_unique<AUDIOEXP::SetupForm>(rs);
        if (sform->exec() > 0){
            edm.updateEntity(*rs);
            edm.all();
        }
    }

}
