#include "goblin.h"

Goblin::Goblin(Player* player, QPoint p):
    Monster(1,p){
    if (rand()%2){
        direction = 1;
    }else{
        direction = -1;
    }

    velocity = QPair<qreal, qreal>(1.5,0);
    acceleration = QPair<qreal,qreal>(0,-0.01);

    QPixmap pixmap = QPixmap(":/Pictures/Monster/Running/MonsterPictures/Goblin_000.png");

    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
    m_player = player;

    frameTimer.setInterval(40);
    connect(&frameTimer, &QTimer::timeout, this, &Goblin::nextFrame);
    frameTimer.start();
}

void Goblin::update(){
    if (health <= 0){
        setupRemove();
        return;
    }

    if (!start && qFabs(scenePos().x()-m_player->scenePos().x()) > 400){
        return;
    }else{
        if (scenePos().x()>=m_player->scenePos().x()){
            addDirection(-2);
        }
        start = true;
    }

    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);

    auto velocity_x = velocity.first*direction;
    auto velocity_y = velocity.second;

    collideTop = false; collideBottom = false; collideLeft = false; collideRight = false;

    foreach (auto item, items) {
        Player *p = qgraphicsitem_cast<Player*>(item);
        if (p != nullptr && ! p->takingDamage ){
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
                    p->minusHealth();
                }
            }else{
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxWid = qMax<qreal>(p->boundingRect().width(), boundingRect().width());

                auto distance_RightLeft = p->sceneBoundingRect().x() + p->boundingRect().width()- sceneBoundingRect().x();

                // This Tile

                if ( 0.1 < distance_RightLeft && distance_RightLeft < maxWid/2.0 ){
                    collideLeft = true;
                    p->push(-2.5);
                }else{
                    collideRight = true;
                    p->push(2.5);
                }

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

                if ( 0 < distance_RightLeft && distance_RightLeft < maxWid/2.0 ){
                    collideLeft = true;
                    m->minusHealth();
                }

                auto distance_LeftRight = sceneBoundingRect().x()+boundingRect().width() -
                        m->sceneBoundingRect().x();

                // Tile this

                if ( 0 < distance_LeftRight && distance_LeftRight < maxWid/2.0 ){
                    collideRight = true;
                    m->minusHealth();
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
                minusHealth();
                return;
            }
        }
    }

    moveBy(velocity_x, velocity_y);

    velocity.first += acceleration.first;
    velocity.second += acceleration.second;

    acceleration = gravityAcceleration;

}

void Goblin::nextFrame(){
    if (m_bRemove || !start){
        return;
    }
    currentIndex++;
    currentIndex %= 6;
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Running/MonsterPictures/Goblin_00"+
                             QString::number(currentIndex) + ".png");
    if (currentIndex == 5){
        pixmap = pixmap.scaled(70,50);
        boundingRect().setWidth(70);
    }else{
        boundingRect().setWidth(50);
        pixmap = pixmap.scaled(50,50);
    }
    setPixmap(pixmap);
}

void Goblin::remove(){
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(50);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &Goblin::animateRemove);
}

void Goblin::setupRemove(){
    m_bRemove = true;
}

void Goblin::animateRemove(){
    if (removeIndex >= 4){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Remove/MonsterPictures/GoblinRemove_"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
    removeIndex++;
}
