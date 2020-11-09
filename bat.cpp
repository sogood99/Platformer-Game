#include "bat.h"
#include "player.h"
#include "monster.h"
#include "goblin.h"

Bat::Bat(QPoint p_A, QPoint p_B, qreal v): Monster(1,p_A){
    auto diff = p_B-p_A;
    qreal d_x = diff.x();
    qreal d_y = diff.y();
    qreal length = sqrt(diff.dotProduct(diff,diff));
    v = qBound(-MAX_VELOCITY, v, MAX_VELOCITY);
    velocity.first = d_x*v/length;
    velocity.second = d_y*v/length;
    totalSteps = length/v;
    if (velocity.first < 0){
        velocity.first *= -1;
        velocity.second *= -1;
        direction = -1;
    }
    direction = 1;
    frameTimer.setInterval(50);
    connect(&frameTimer, &QTimer::timeout, this, &Bat::nextFrame);
    frameTimer.start();
    fireTimer.setInterval(2200);
    fireTimer.start();
    connect(&fireTimer, &QTimer::timeout, this, &Bat::fire);
}


void Bat::update(){
    // Detects collision and moves

    if (health <= 0){
        setupRemove();
        if (bullet != nullptr){
            bullet->setupRemove();
            bullet->remove();
            bullet = nullptr;
        }
        return;
    }

    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (auto item, items) {
        Player *p = qgraphicsitem_cast<Player*>(item);
        if (p != nullptr && ! p->takingDamage){
            qDebug()<<"p";
            auto rec = p->sceneBoundingRect()&sceneBoundingRect();
            if (2*rec.width()*rec.height() >= sceneBoundingRect().width()*sceneBoundingRect().height()){
                minusHealth();
                if (scenePos().x() > p->scenePos().x()){
                    p->push(-1.5);
                }else{
                    p->push(1.5);
                }
                if (scenePos().y() > p->scenePos().y()){
                    p->bounce(-1.5);
                }else{
                    p->bounce(1.5);
                }
            }
        }else if (Bat *b = qgraphicsitem_cast<Bat*>(item)){
            qDebug()<<"b";
            if (direction == 1){
                addDirection(-2);
            }else if (direction == -1){
                addDirection(2);
            }
        }else if (Tiles *t = qgraphicsitem_cast<Tiles*>(item)){
            qDebug()<<"t";
            QRectF intersection = this->sceneBoundingRect() & t->sceneBoundingRect();
            if (intersection.width() >= intersection.height()){

                auto maxHei = qMax<qreal>(t->boundingRect().height(), boundingRect().height());

                auto distance_topDown = sceneBoundingRect().y()+boundingRect().height() - t->sceneBoundingRect().y();

                if (0 < distance_topDown && distance_topDown < maxHei/2.0 ){
                    collideBottom = true;
                    moveBy(0, -distance_topDown-0.1);
                }

                auto distance_downTop = t->sceneBoundingRect().y()+t->boundingRect().height() - sceneBoundingRect().y();
                if ( 0 < distance_downTop && distance_downTop < maxHei/2.0 ){
                    collideTop = true;
                    moveBy(0, distance_downTop + 0.1);
                }
            }else{

                auto maxWid = qMax<qreal>(t->boundingRect().width(), boundingRect().width());

                auto distance_RightLeft = t->sceneBoundingRect().x() + t->boundingRect().width()- sceneBoundingRect().x();

                // This Tile

                if ( 0.1 < distance_RightLeft && distance_RightLeft < maxWid/2.0 ){
                    collideLeft = true;
                    moveBy(distance_RightLeft+0.1, 0);
                }

                auto distance_LeftRight = sceneBoundingRect().x()+boundingRect().width() - t->sceneBoundingRect().x();

                // Tile this

                if ( 0.1 < distance_LeftRight && distance_LeftRight < maxWid/2.0 ){
                    collideRight = true;
                    moveBy(-distance_LeftRight-0.1, 0);
                }
            }
            if (direction == 1){
                addDirection(-2);
            }else if (direction == -1){
                addDirection(2);
            }
            currentSteps = 0;
        }
    }

    if (currentSteps > totalSteps){
        currentSteps = 0;
        if (direction == 1){
            addDirection(-2);
        }else if (direction == -1){
            addDirection(2);
        }
    }else{
        currentSteps++;
        moveBy(direction*velocity.first,direction*velocity.second);
    }
    if (bullet != nullptr){
        if (bullet->m_bRemove){
            bullet->setupRemove();
            bullet->remove();
            bullet = nullptr;
        }else{
            bullet->update();
        }
    }
}

void Bat::nextFrame(){
    // Provides animation
    if (m_bRemove){
        return;
    }
    currentIndex++;
    currentIndex %= 8;
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Running/MonsterPictures/Bat_"+
                             QString::number(currentIndex) + ".png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
}

void Bat::fire(){
    // Shoots monster bullets
    if (bullet == nullptr && !m_bRemove){
        QPointF p = sceneBoundingRect().topRight() + QPointF(0,25) - QPointF(0,17.5);
        if (spriteReversed){
            p -= QPointF(50, 0);
        }
        bullet = new MonsterBullet(p.toPoint(), (!spriteReversed ? 1 : -1), velocity);
        scene()->addItem(bullet);
    }
}

void Bat::animateRemove(){
    // Animation before deleting
    if (removeIndex >= 4){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Monster/Remove/MonsterPictures/BatRemove_"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(50,50);
    setPixmap(pixmap);
    removeIndex++;
}

void Bat::setupRemove(){
    if (!m_bRemove){
        m_bRemove = true;
        if (bullet){
            disconnect(&fireTimer, &QTimer::timeout, this, &Bat::fire);
            fireTimer.stop();
            scene()->removeItem(bullet);
            bullet->setupRemove();
            bullet->remove();
            bullet = nullptr;
        }
    }
}

void Bat::remove(){
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(50);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &Bat::animateRemove);
}
