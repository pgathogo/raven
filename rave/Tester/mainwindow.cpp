#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <filesystem>
#include <QPushButton>
#include <QDebug>

#include "../audio/audiofile.h"
#include "../audio/audiotool.h"
#include "../audiolib/headers/cueeditor.h"

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnCue, &QPushButton::clicked, this, &MainWindow::open_editor);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_editor()
{

    AudioFile audio_file("D:\\home\\PMS\\RavenTest\\Audio\\let_me_go.mp3");

    if (audio_file.is_valid()){
        Marker m;
        ADFRepository adf_repo;
        AudioTool audio_tool;

        audio_file.set_audio_title("Tester Audio");
        audio_file.set_artist_name("Test Artist");
        audio_file.set_marker(m);
        audio_file.set_audio_lib_path("D:\\home\\PMS\\RavenTest\\OggLib\\");

        if (fs::exists(audio_file.adf_file())){
            adf_repo.read_markers(audio_file.audio_file());
            auto af_markers = audio_file.marker();

            m.start_marker = af_markers.start_marker;
            m.fade_in = af_markers.fade_in;
            m.intro = af_markers.intro;
            m.extro = af_markers.extro;
            m.fade_out = af_markers.fade_out;
            m.end_marker = af_markers.end_marker;
        }


        if(!fs::exists(audio_file.wave_file())){
            qDebug() << "To generate wave file ...";
            audio_tool.generate_wave_file(audio_file.audio_file(), audio_file.wave_file());
            qDebug() << "Generated wave file ...";
        }

        audio_file.set_ogg_filename("d:/Music/Studi1/test.ogg");

        qDebug() << "To open cue editor ...";
        CueEditor* cue_editor = new CueEditor(audio_file);
        qDebug() << "After openning cue editor ...";

        if (cue_editor->editor() == 1){
            audio_file.set_marker(cue_editor->marker());
            adf_repo.write(audio_file);
        }


    }

}
