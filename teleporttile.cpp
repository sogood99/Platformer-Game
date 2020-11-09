#include "teleporttile.h"

TeleportTile::TeleportTile(QPoint pointA, QPoint pointB, TeleportTile* other):
    Tiles(pointA){

    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/TeleportTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);

    if (!other){
        otherTile = new TeleportTile(pointB, pointA, this);
    }else{
        otherTile = other;
    }
    teleportSound = new QMediaPlayer(this);
    teleportSound->setMedia(QUrl("qrc:/Music/Music/TeleportSound.mp3"));
}

void TeleportTile::collidedWith(Player *p){
    QRectF intersection = sceneBoundingRect() & p->sceneBoundingRect();

    if (intersection.width() >= intersection.height()&& p->boundingRect().width()/2<=intersection.width()){
        playTeleportSound();
        QPointF diff = otherTile->scenePos() - p->sceneBoundingRect().topLeft();
        p->moveBy(diff.x(),diff.y());
        if (p->m_velocity.second > 0){
            p->m_velocity.second *= -0.9;
        }else{
            p->m_velocity.second *= 0.9;
        }
        p->moveBy(0,-151);
    }
}

void TeleportTile::update(){
    if (!otherTile->scene()){
        scene()->addItem(otherTile);
    }
}

void TeleportTile::playTeleportSound(){
    if (teleportSound->state() == QMediaPlayer::PlayingState){
        teleportSound->setPosition(0);
    }else if (teleportSound->state() == QMediaPlayer::StoppedState){
        teleportSound->play();
    }
}

