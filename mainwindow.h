#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "mapeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MapSelector;

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void home();
    void addMap();
    void editMap(QString map);
    void playMap(QString map);
    void finishLoading();

public slots:
    void buttonSound();
    void updateLoading();
private slots:
    void on_PlayButton_clicked();

private:
    Ui::MainWindow *ui;
    QWidget* homeWidget = nullptr;
    Game* game = nullptr;
    MapEditor* mapEditor = nullptr;
    MapSelector* mapSelector = nullptr;
    QMediaPlayer* menuMusicPlayer = nullptr;
    QMediaPlayer* buttonClickPlayer = nullptr;
    QString gameMapName = "";
    QWidget* loadingWidget = nullptr;
    QLabel* loadingLabel = nullptr;
    QTimer loadingTimer;
};
#endif // MAINWINDOW_H
