#include "collectable.h"

Collectable::Collectable(QPoint p){
    moveBy(p.x(),p.y());
}

void Collectable::remove(){
    deleteLater();
}

void Collectable::setupRemove(){
    m_bRemove = true;
}


Gold::Gold(QPoint p):Collectable(p){
    QPixmap pixmap(":/Pictures/ConsumablePictures/ConsumablePictures/Coin/Coin_0.png");
    pixmap = pixmap.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(pixmap);

    frameTimer.setInterval(50);
    frameTimer.start();
    connect(&frameTimer, &QTimer::timeout, this, &Gold::nextFrame);
}

void Gold::nextFrame(){
    if (m_bRemove){
        return;
    }
    currentCount++;
    currentCount%=16;
    QPixmap pixmap(":/Pictures/ConsumablePictures/ConsumablePictures/Coin/Coin_"+
                   QString::number(currentCount)+".png");
    pixmap = pixmap.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(pixmap);
}

void Gold::collected(Player *player){
    // Used by player when collided
    player->score++;
    setupRemove();
}

Potion::Potion(QPoint p):Collectable(p){
    QPixmap pixmap(":/Pictures/ConsumablePictures/ConsumablePictures/Potion/Potion.png");
    pixmap = pixmap.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(pixmap);
    frameTimer.setInterval(50);
    frameTimer.start();
    connect(&frameTimer, &QTimer::timeout, this, &Potion::nextFrame);
}

void Potion::nextFrame(){
    if (m_bRemove){
        return;
    }
    if (0 <= direction){
        if (direction <= 10){
            direction++;
            moveBy(0,direction*0.2);
        }else{
            direction = -1;
        }
    }else{
        if (direction >= -10){
            direction--;
            moveBy(0,direction*0.2);
        }else{
            direction = 1;
        }
    }
}

void Potion::collected(Player *player){
    // Used by player when collided
    player->health++;
    setupRemove();
}


BulletCollect::BulletCollect(QPoint p):Collectable(p){
    QPixmap pixmap(":/Pictures/ConsumablePictures/ConsumablePictures/Bullet/Bullet.png");
    pixmap = pixmap.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(pixmap);

    frameTimer.setInterval(50);
    frameTimer.start();
    connect(&frameTimer, &QTimer::timeout, this, &BulletCollect::nextFrame);
}

void BulletCollect::nextFrame(){
    if (m_bRemove){
        return;
    }
    if (0 <= direction){
        if (direction <= 10){
            direction++;
            moveBy(0,direction*0.2);
        }else{
            direction = -1;
        }
    }else{
        if (direction >= -10){
            direction--;
            moveBy(0,direction*0.2);
        }else{
            direction = 1;
        }
    }
}

void BulletCollect::collected(Player *player){
    // Used by player when collided
    player->bulletCount++;
    setupRemove();
}
