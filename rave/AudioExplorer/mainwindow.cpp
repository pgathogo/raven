
#include <QDebug>
#include <QStandardItemModel>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "datatoolbar.h"

#include "../audio/audiolibrary.h"
#include "../audio/audio.h"
#include "../audio/artist.h"
#include "../audio/audiolibitem.h"
#include "../audio/audiotool.h"
#include "../framework/treeviewmodel.h"
#include "../framework/ravenexception.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_artist_toolbar = new DataToolBar();
    ui->hlArtist->addWidget(m_artist_toolbar);

    connect(m_artist_toolbar->add_button(), &QPushButton::clicked, this, &MainWindow::add_artist);
    connect(m_artist_toolbar->edit_button(), &QPushButton::clicked, this, &MainWindow::edit_artist);
    connect(m_artist_toolbar->delete_button(), &QPushButton::clicked, this, &MainWindow::delete_artist);

    m_genre_toolbar = new DataToolBar();
    ui->hlGenre->addWidget(m_genre_toolbar);

    m_audio_entity_data_model = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Audio>());
    m_artist_entity_data_model = std::make_unique<EntityDataModel>(std::make_unique<AUDIO::Artist>());

    setup_audio_folders();

    m_tracks_model = new QStandardItemModel(this);
    create_track_view_headers();
    set_track_view_column_width();

    m_audio_lib_item = std::make_unique<AUDIO::AudioLibItem>(m_tracks_model);

    ui->tvTracks->horizontalHeader()->setStretchLastSection(true);
    ui->tvTracks->setModel(m_tracks_model);
    ui->tvTracks->verticalHeader()->setVisible(false);

    m_artist_model = new QStandardItemModel(this);
    create_artist_view_headers();

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

void MainWindow::setup_artists()
{

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
    m_artist_model->setHorizontalHeaderItem(0, new QStandardItem("Artist"));
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
    fetch_audio(filter_str);
}

void MainWindow::fetch_audio(const std::string filter)
{
    m_audio_entity_data_model->clearEntities();
    m_audio_entity_data_model->search(filter);
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


void MainWindow::add_artist()
{

}
void MainWindow::edit_artist()
{
}

void MainWindow::delete_artist()
{
}
