#include "jumptile.h"

JumpTile::JumpTile(QPoint p): Tiles(p, 0, QPair<qreal,qreal>(0,-3)){
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/JumpTile_0.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);

    frameTimer.setInterval(45);
    frameTimer.start();
    connect(&frameTimer, &QTimer::timeout, this, &JumpTile::nextFrame);
}

void JumpTile::nextFrame(){
    currentFrame++;
    currentFrame %= 4;
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/JumpTile_"+
                   QString::number(currentFrame)+".png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}
