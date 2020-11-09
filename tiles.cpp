#include "tiles.h"

Tiles::Tiles(QPoint p, qreal friction,
             QPair<qreal, qreal> velocity,
             QPair<qreal, qreal> acceleration){

    Tiles::friction = friction;
    Tiles::acceleration = acceleration;
    Tiles::velocity = velocity;
    boundingRect().setX(0);
    boundingRect().setY(0);
    boundingRect().setWidth(80);
    boundingRect().setHeight(80);

    moveBy(p.x(),p.y());
}

void Tiles::update(){

}

void Tiles::setupRemove(){
    m_bRemove = true;
}

void Tiles::remove(){
    deleteLater();
}

void Tiles::collidedWith(Player *){

}

void Tiles::collidedWith(Monster *){

}

void Tiles::collidedWith(Bullet *){

}
