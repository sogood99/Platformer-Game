#ifndef UTILS_H
#define UTILS_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPair>
#include <QScrollBar>
#include <QtMath>
#include <QPropertyAnimation>
#include <QBitmap>
#include <QVector>
#include <QGraphicsProxyWidget>
#include <QMediaPlayer>

// Contains useful includes, defines and such.

#define TIME_STEP 5
#define GRAVITY 0.05
#define MAX_VELOCITY 3.0
#define EPSILON 0.1
#define X_MAX 3000
#define X_MIN -2000
#define Y_MAX 1000
#define Y_MIN -1000

enum state{
    idle,
    running,
    jumping,
    teleporting,
    dying
};

enum GameObject{
    go_Empty,
    go_StartFlag,
    go_EndFlag,
    go_Gold,
    go_Potion,
    go_BulletCollect,
    go_GrassTile,
    go_SandTile,
    go_TeleportTile,
    go_MovingTile,
    go_TrapTile,
    go_JumpTile,
    go_CollapseTile,
    go_Mushroom,
    go_Goblin,
    go_Bat,
    go_LINKED
};


#endif
