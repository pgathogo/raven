#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "../audiolib/headers/cueeditor.h"
#include "../audiolib/headers/audioplayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

