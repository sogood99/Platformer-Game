#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mapselector.h"
#include <QMediaPlaylist>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setCentralWidget(ui->MainStack);
    setFixedSize(800,600);

    QMediaPlaylist *playlist = new QMediaPlaylist(this);
    playlist->addMedia(QUrl("qrc:/Music/Music/GameMusic.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    menuMusicPlayer = new QMediaPlayer(this);
    menuMusicPlayer->setPlaylist(playlist);
    menuMusicPlayer->play();

    buttonClickPlayer = new QMediaPlayer(this);
    buttonClickPlayer->setMedia(QUrl("qrc:/Music/Music/SelectSound.mp3"));


    QLabel* background = ui->HomePage->findChild<QLabel*>("Background");
    background->setStyleSheet("background-color:rgba(255,255,255,100)");
    QMovie *movie = new QMovie(":/Gif/Gifs/Sakura.gif");
    movie->setScaledSize(QSize(900,900));
    background->setMovie(movie);
    movie->start();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::home(){
    // Used to get back to home
    if (game != nullptr){
        ui->MainStack->removeWidget(game);
        game->hide();
        game->deleteLater();
        game = nullptr;
        menuMusicPlayer->play();
    }else if (mapEditor != nullptr){
        ui->MainStack->removeWidget(mapEditor);
        mapEditor->hide();
        mapEditor->deleteLater();
        mapEditor = nullptr;
    }else if (mapSelector != nullptr){
        ui->MainStack->removeWidget(mapSelector);
        mapSelector->hide();
        mapSelector->deleteLater();
        mapSelector = nullptr;
    }
    ui->MainStack->setCurrentIndex(0);
}

void MainWindow::addMap(){
    // Used by mapselector to create new maps
    home();
    mapEditor = new MapEditor(this);
    ui->MainStack->insertWidget(1,mapEditor);
    ui->MainStack->setCurrentIndex(1);
}

void MainWindow::editMap(QString map){
    // Used by mapselector to edit old maps
    home();
    mapEditor = new MapEditor(this);
    ui->MainStack->insertWidget(1,mapEditor);
    ui->MainStack->setCurrentIndex(1);
    mapEditor->open(map);
}

void MainWindow::playMap(QString map){
    // Used by mapselector to play map
    home();
    menuMusicPlayer->stop();
    gameMapName = map;
    loadingTimer.setInterval(25);
    loadingTimer.start();
    connect(&loadingTimer, &QTimer::timeout, this, &MainWindow::updateLoading);
    loadingWidget = new QWidget(this);
    loadingLabel = new QLabel(loadingWidget);
    QLabel* finishLabel = new QLabel(loadingWidget);
    finishLabel->setGeometry(300,300,200,50);
    loadingLabel->raise();
    loadingLabel->setStyleSheet("background-color:green");
    loadingLabel->setGeometry(300,300,0,50);
    finishLabel->setStyleSheet("background-color:white; border: 1px solid black;");
    ui->MainStack->insertWidget(1,loadingWidget);
    ui->MainStack->setCurrentIndex(1);
}

void MainWindow::updateLoading(){
    // Animate loading screen
    loadingLabel->setGeometry(300,300,loadingLabel->width()+10,50);
    if (loadingLabel->width() > 200){
        disconnect(&loadingTimer, &QTimer::timeout, this, &MainWindow::updateLoading);
        loadingTimer.stop();
        finishLoading();
    }
}

void MainWindow::finishLoading(){
    // Called when loading finished
    ui->MainStack->removeWidget(loadingWidget);
    loadingWidget->deleteLater();
    loadingWidget = nullptr;
    loadingLabel = nullptr;
    game = new Game(gameMapName, this);
    ui->MainStack->insertWidget(1,game);
    ui->MainStack->setCurrentIndex(1);
    game->setFocus();
}

void MainWindow::buttonSound(){
    // Sound effects for button
    if (buttonClickPlayer->state() == QMediaPlayer::PlayingState){
        buttonClickPlayer->setPosition(0);
    }else if (buttonClickPlayer->state() == QMediaPlayer::StoppedState){
        buttonClickPlayer->play();
    }
}

void MainWindow::on_PlayButton_clicked(){
    mapSelector = new MapSelector(this);
    ui->MainStack->insertWidget(1,mapSelector);
    ui->MainStack->setCurrentIndex(1);
    buttonSound();
}
