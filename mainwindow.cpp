#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMediaPlayer>
#include <QStandardPaths>
#include <QTime>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player.setVolume(50);

    /* Łączenie sygnałów i slotów */

    connect(&player, &QMediaPlayer::positionChanged, this, &MainWindow::onMediaPositionChanged);
    connect(&player, &QMediaPlayer::durationChanged, this, &MainWindow::onMediaDurationChanged);
    connect(&player, &QMediaPlayer::videoAvailableChanged, this, &MainWindow::onVideoAvailableChanged);
    connect(ui->actionStart_Pauza, &QAction::triggered, this, &MainWindow::on_playButton_pressed);
    connect(ui->actionForward, &QAction::triggered, this, &MainWindow::on_forwardButton_pressed);
    connect(ui->actionRewind, &QAction::triggered, this, &MainWindow::on_backwardButton_pressed);
    connect(ui->actionVolumeDown, &QAction::triggered, this, [this] {
        ui->volumeSlider->setValue(ui->volumeSlider->value() - 5);
    });
    connect(ui->actionVolumeUp, &QAction::triggered, this, [this] {
        ui->volumeSlider->setValue(ui->volumeSlider->value() + 5);
    });

    ui->stackedWidget->setCurrentIndex(0);
    ui->videoView->show();
    player.setVideoOutput(ui->videoView);

    ui->musicPlaceholder->load(QString(":/images/images/music-placeholder.svg"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playButton_pressed()
{
    if (player.mediaStatus() == QMediaPlayer::NoMedia)
        return;

    switch(player.state()) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        player.play();
        ui->playButton->setIcon(QIcon(":/images/images/pause.svg"));
        break;
    case QMediaPlayer::PlayingState:
        player.pause();
        ui->playButton->setIcon(QIcon(":/images/images/right-arrow.svg"));
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Otwórz plik"),
                                                    QStandardPaths::writableLocation(QStandardPaths::MoviesLocation),
                                                    tr("Wideo lub audio (*.mp3 *.mp4 *.mkv *.ogg)"));
    if(fileName == "")
        return;

    player.setMedia(QUrl::fromLocalFile(fileName));
    ui->playerProgress->setMinimum(0);
    ui->playerProgress->setValue(0);
    ui->playerProgress->setTextVisible(true);
    ui->playerProgress->setFormat("00:00");

    onVideoAvailableChanged();

    ui->startLabel->setText("00:00:00");

}

void MainWindow::onVideoAvailableChanged() {
    if(!player.isVideoAvailable()) {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void MainWindow::on_forwardButton_pressed()
{
    player.setPosition((player.position() + 5000 <= player.duration())? player.position() + 5000 : player.duration());
}

void MainWindow::onMediaPositionChanged() {
    QTime time(0,0,0,0);
    ui->playerProgress->setValue(player.position() / 1000);
    ui->playerProgress->setFormat(time.addMSecs(player.position()).toString());
    if(player.position() == player.duration())
        ui->playButton->setIcon(QIcon(":/images/images/right-arrow.svg"));
}

void MainWindow::onMediaDurationChanged() {
    ui->playerProgress->setMaximum(player.duration() / 1000);
    QTime time(0, 0, 0, 0);
    ui->endLabel->setText(time.addMSecs(player.duration()).toString("hh:mm:ss"));
}

void MainWindow::on_backwardButton_pressed()
{
    player.setPosition(player.position() - 5000);
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{
    player.setVolume(value);
}

void MainWindow::on_actionQuit_triggered()
{
    exit(EXIT_SUCCESS);
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,
                             "O programie",
                             "Autorami programu są Seweryn Pajor i Arkadiusz Hamiga z klasy 2d.<br>"
                             "Projekt powstał w celu zaprezentowania frameworka Qt na lekcji podstaw programowania.");
}
