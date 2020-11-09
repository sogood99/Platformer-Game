#include "traptile.h"
#include "player.h"
#include "monster.h"
#include "goblin.h"

TrapTile::TrapTile(QPoint p):Tiles(p){
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/GrassTile.png");
    pixmap = pixmap.scaled(80,80);
    setPixmap(pixmap);
}

void TrapTile::setActive(){
    isActive = true;
    QPixmap pixmap(":/Pictures/TilePictures/TilePictures/Spikes.png");
    pixmap = pixmap.scaled(80,80);
    QRect rect(0,40,80,40);
    pixmap = pixmap.copy(rect);
    setPixmap(pixmap);
    moveBy(0,40);
}

void TrapTile::collidedWith(Player *p){
    if (isActive){
        p->minusHealth();
        p->bounce(-3);
        p->moveBy(0,-20);
        return;
    }

    QRectF intersection = sceneBoundingRect() & p->sceneBoundingRect();

    if (intersection.width() >= intersection.height()&&
            p->boundingRect().width()/1.5<=intersection.width()){
        setActive();
    }
}

void TrapTile::collidedWith(Monster *m){
    if (isActive){
        m->minusHealth();
        m->bounce(-3);
        m->moveBy(0,-20);
        return;
    }

    QRectF intersection = sceneBoundingRect() & m->sceneBoundingRect();

    if (intersection.width() >= intersection.height()&&
            m->boundingRect().width()/1.5<=intersection.width()){
        setActive();
    }
}
