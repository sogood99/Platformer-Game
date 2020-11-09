#include "monster.h"

Monster::Monster(int initHealth, QPoint p,
                 QPair<qreal,qreal> initVelocity, QPair<qreal,qreal> jump_on_Velocity){
    boundingRect().setRect(-5,-5,60,60);
    health = initHealth;
    velocity = initVelocity;
    jumpOnVelocity = jump_on_Velocity;
    moveBy(p.x(),p.y());
}

void Monster::bounce(qreal vel){
    velocity.second = vel;
}

void Monster::push(qreal vel){
    velocity.first = vel;
}

void Monster::addDirection(int dir){
    direction += dir;
    direction = qBound(-1,direction,1);
    setTransformOriginPoint(25, 50);
    if (direction == 1){
        setTransform(QTransform());
        spriteReversed = false;
    }else if (direction == -1){
        QTransform trans;
        trans.scale(-1,1);
        trans.translate(-50,0);
        setTransform(trans);
        spriteReversed = true;
    }
}

void Monster::minusHealth(){
    health--;
}

void Monster::update(){

}

void Monster::nextFrame(){

}

void Monster::remove(){

}

void Monster::setupRemove(){
    m_bRemove = true;
}

