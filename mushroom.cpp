#include "mushroom.h"

Mushroom::Mushroom(QPoint p): Monster(1,p){
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Running/MonsterPictures/Mushroom_0.png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
    frameTimer.setInterval(50);
    connect(&frameTimer, &QTimer::timeout, this, &Monster::nextFrame);
    frameTimer.start();
    direction = 1;
}

void Mushroom::update(){
    // Detects collision and moves

    if (health <= 0){
        setupRemove();
        return;
    }

    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);

    auto velocity_x = velocity.first*direction;
    auto velocity_y = velocity.second;

    collideTop = false; collideBottom = false; collideLeft = false; collideRight = false;

    foreach (auto item, items) {
        Player *p = qgraphicsitem_cast<Player*>(item);
        if (p != nullptr && ! p->takingDamage){
            QRectF intersection = this->sceneBoundingRect() & p->sceneBoundingRect();

            if (intersection.width() >= intersection.height()){

                auto maxHei = qMax<qreal>(p->boundingRect().height(), boundingRect().height());

                auto distance_topDown = sceneBoundingRect().y()+boundingRect().height() -
                        p->sceneBoundingRect().y();

                if ( 0 < distance_topDown && distance_topDown < maxHei/5.0 ){
                    collideBottom = true;
                    p->bounce(2.5);
                    p->minusHealth();
                }

                auto distance_downTop = p->sceneBoundingRect().y()+
                        p->boundingRect().height()-sceneBoundingRect().y();
                if ( 0 < distance_downTop && distance_downTop < maxHei/5.0 ){
                    collideTop = true;
                    p->bounce(-2.5);
                    minusHealth();
                }
            }else{
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                // This Tile
                p->minusHealth();
            }
        }

        if (Monster *m = qgraphicsitem_cast<Monster*>(item)){
            QRectF intersection = this->sceneBoundingRect() & m->sceneBoundingRect();

            if (intersection.width() >= intersection.height()){
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxHei = qMax<qreal>(m->boundingRect().height(), boundingRect().height());

                auto distance_topDown = sceneBoundingRect().y()+boundingRect().height() -
                        m->sceneBoundingRect().y();

                if ( 0 < distance_topDown && distance_topDown < maxHei/6.0 ){
                    collideBottom = true;
                    moveBy(0, -distance_topDown+0.1);
                    velocity.second = qMin<qreal>(velocity.second, 0);
                    acceleration.second = qMin<qreal>(acceleration.second, 0);
                }

                auto distance_downTop = m->sceneBoundingRect().y()+
                        m->boundingRect().height()-sceneBoundingRect().y();
                if ( 0 < distance_downTop && distance_downTop < maxHei/6.0 ){
                    collideTop = true;
                    moveBy(0, distance_downTop - 0.1);
                    velocity.second = (qMax<qreal>(velocity.second, 0));
                    acceleration.second = (qMax<qreal>(acceleration.second, 0));
                }
            }else{
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxWid = qMax<qreal>(m->boundingRect().width(), boundingRect().width());

                auto distance_RightLeft = m->sceneBoundingRect().x() +
                        m->boundingRect().width()- sceneBoundingRect().x();

                // This Tile

                if ( 0 < distance_RightLeft && distance_RightLeft < maxWid/6.0 ){
                    collideLeft = true;
                    moveBy(distance_RightLeft-0.1, -0.2);
//                    velocity_x = (qMax<qreal>(velocity_x, 0));
                    addDirection(2);
                    acceleration.first = (qMax<qreal>(acceleration.first, 0));
                }

                auto distance_LeftRight = sceneBoundingRect().x()+boundingRect().width() -
                        m->sceneBoundingRect().x();

                // Tile this

                if ( 0 < distance_LeftRight && distance_LeftRight < maxWid/6.0 ){
                    collideRight = true;
                    moveBy(-distance_LeftRight+0.1, -0.2);
                    addDirection(-2);
                    acceleration.first = (qMin<qreal>(acceleration.first, 0));
                }
            }
        }

        if (Tiles *t = qgraphicsitem_cast<Tiles*>(item)){
            /*
             * player
             * tile
            */

            QRectF intersection = this->sceneBoundingRect() & t->sceneBoundingRect();

            if (intersection.width() >= intersection.height()){
                velocity_x = velocity_x - t->friction*velocity_x;
                if (qFabs(t->velocity.second)>EPSILON){
                    bounce(t->velocity.second);
                    velocity_y = velocity.second;
                }
                if (qFabs(t->velocity.first)>EPSILON){
                    velocity_x += t->velocity.first;
                    velocity_x -= t->friction*velocity_x;
                }
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxHei = qMax<qreal>(t->boundingRect().height(), boundingRect().height());

                auto distance_topDown = sceneBoundingRect().y()+boundingRect().height() - t->sceneBoundingRect().y();

                if ( 0 < distance_topDown && distance_topDown < maxHei/5.0 ){
                    collideBottom = true;
                    moveBy(0, -distance_topDown+0.1);
                    velocity.second = qMin<qreal>(velocity.second, 0);
                    acceleration.second = qMin<qreal>(acceleration.second, 0);
                }

                auto distance_downTop = t->sceneBoundingRect().y()+t->boundingRect().height() - sceneBoundingRect().y();
                if ( 0 < distance_downTop && distance_downTop < maxHei/6.0 ){
                    collideTop = true;
                    moveBy(0, distance_downTop - 0.1);
                    velocity.second = (qMax<qreal>(velocity.second, 0));
                    acceleration.second = (qMax<qreal>(acceleration.second, 0));
                }
            }else{
                velocity_y = velocity_y - t->friction*velocity_y;
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxWid = qMax<qreal>(t->boundingRect().width(), boundingRect().width());

                auto distance_RightLeft = t->sceneBoundingRect().x() + t->boundingRect().width()- sceneBoundingRect().x();

                // This Tile

                if ( 0 < distance_RightLeft && distance_RightLeft < maxWid/5.0 ){
                    collideLeft = true;
                    moveBy(distance_RightLeft-0.1, -0.2);
                    addDirection(2);
//                    velocity_x = (qMax<qreal>(velocity_x, 0));
                    acceleration.first = (qMax<qreal>(acceleration.first, 0));
                }

                auto distance_LeftRight = sceneBoundingRect().x()+boundingRect().width() - t->sceneBoundingRect().x();

                // Tile this

                if ( 0 < distance_LeftRight && distance_LeftRight < maxWid/5.0 ){
                    collideRight = true;
                    moveBy(-distance_LeftRight+0.1, -0.2);
                    addDirection(-2);
//                    velocity_x = (qMin<qreal>(velocity_x, 0));
                    acceleration.first = (qMin<qreal>(acceleration.first, 0));
                }
            }
        }
    }

    moveBy(velocity_x, velocity_y);

    velocity.first += acceleration.first;
    velocity.second += acceleration.second;

    acceleration = gravityAcceleration;
}

void Mushroom::nextFrame(){
    if (m_bRemove){
        return;
    }
    currentIndex++;
    currentIndex %= 8;
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Running/MonsterPictures/Mushroom_"+
                             QString::number(currentIndex) + ".png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
}

void Mushroom::remove(){
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(50);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &Mushroom::animateRemove);
}

void Mushroom::animateRemove(){
    if (removeIndex >= 4){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Remove/MonsterPictures/MushroomRemove_"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
    removeIndex++;
}
