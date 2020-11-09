#ifndef GAME_H
#define GAME_H

#include <QLinkedList>
#include <QElapsedTimer>
#include "utils.h"
#include "player.h"
#include "tiles.h"
#include "monster.h"
#include "mushroom.h"
#include "collapsetile.h"
#include "grasstile.h"
#include "sandtile.h"
#include "jumptile.h"
#include "movingtile.h"
#include "labels.h"
#include "goblin.h"
#include "teleporttile.h"
#include "bullet.h"
#include "bat.h"
#include "collectable.h"
#include "traptile.h"
#include "flags.h"
#include "gameobj.h"

class MainWindow;

class Game: public QGraphicsView{
    /*
     * Child of QGraphicsView, used as User Interface
     */
    Q_OBJECT
public:
    Game(QString mapName,MainWindow* parent = nullptr);
    virtual ~Game();
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void movePlayer();
    void checkTimer();
    void loadGame(QString file_name);
    void gameWin();
    void gameLost();
    bool gameWon = false;
    void initalizeLinkedLists();
    void updateItems();
    void home();
public:
    StartFlag* startFlag = nullptr;
    EndFlag* endFlag = nullptr;
    Player* m_player = nullptr;
    QLinkedList<GameObj*> gameObjList;

public slots:
    void quit();
    void restart();
private:
    QTimer m_timer;
    int leftRight_Border = 100;
    int topDown_Border = 150;
    int widthSize = 800;
    int heightSize = 600;
    Labels* labels = nullptr;
    QElapsedTimer timeUsed;
    QString mapName;
    MainWindow* mainWindow = nullptr;
    QMediaPlayer* gameBgmPlayer = nullptr;
    QMediaPlayer* gameLostSound = nullptr;
    QMediaPlayer* gameWonSound = nullptr;
};

#endif // GAME_H
