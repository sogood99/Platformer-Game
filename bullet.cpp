#include "bullet.h"
#include "tiles.h"
#include "monster.h"
#include "goblin.h"
#include "player.h"
#include "bat.h"

Bullet::Bullet(QPoint p, int dir, QPair<qreal, qreal> addVel){
    boundingRect().setRect(0,0,35,35);
    position.first = p.x();
    position.second = p.y();
    moveBy(p.x(),p.y());
    direction = dir;
    m_CountDownTimer.setInterval(1500);
    connect(&m_CountDownTimer, &QTimer::timeout, this, &Bullet::setupRemove);
    m_CountDownTimer.start();
    velocity = QPair<qreal,qreal>(qBound(-MAX_VELOCITY,2.5+addVel.first,MAX_VELOCITY),0);
}

void Bullet::update(){
    // for positional updates
    if (!m_bRemove){
        moveBy(velocity.first*direction, velocity.second*direction);
    }
}

void Bullet::setupRemove(){
    disconnect(&m_CountDownTimer, &QTimer::timeout, this, &Bullet::setupRemove);
    m_bRemove = true;
}

void Bullet::remove(){
    deleteLater();
}

PlayerBullet::PlayerBullet(QPoint p, int dir, QPair<qreal, qreal> addVel):Bullet(p,dir, addVel){
    QPixmap pixmap(":/Pictures/Bullet/BulletPictures/PlayerBullet0.png");
    pixmap = pixmap.scaled(35,35);
    setPixmap(pixmap);
}

void PlayerBullet::update(){
    // Detects collision and moves
    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (auto item, items) {
        if (Monster* m = qgraphicsitem_cast<Monster*>(item)){
            m->minusHealth();
            setupRemove();
            return;
        }else if (Bat* b = qgraphicsitem_cast<Bat*>(item)){
            b->minusHealth();
            setupRemove();
            return;
        }else if (Tiles* t = qgraphicsitem_cast<Tiles*>(item)){
            t->collidedWith(this);
            setupRemove();
            return;
        }
    }
    Bullet::update();
}

void PlayerBullet::remove(){
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(50);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &PlayerBullet::animateRemove);
}

void PlayerBullet::animateRemove(){
    // Last animation before deleting
    if (removeIndex >= 7){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Bullet/BulletPictures/PlayerBullet"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(35,35);
    setPixmap(pixmap);
    removeIndex++;
}

MonsterBullet::MonsterBullet(QPoint p, int dir, QPair<qreal, qreal> addVel):
    Bullet(p,dir, addVel){
    QPixmap pixmap(":/Pictures/Bullet/BulletPictures/BatBullet0.png");
    pixmap = pixmap.scaled(35,35);
    setPixmap(pixmap);
}

void MonsterBullet::update(){
    // Detects collision and moves
    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);
    foreach (auto item, items) {
        if (Player* p = qgraphicsitem_cast<Player*>(item)){
            p->minusHealth();
            setupRemove();
            return;
        }else if (Tiles* t = qgraphicsitem_cast<Tiles*>(item)){
            t->collidedWith(this);
            setupRemove();
            return;
        }
    }
    Bullet::update();
}

void MonsterBullet::remove(){
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(50);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &MonsterBullet::animateRemove);
}

void MonsterBullet::animateRemove(){
    // Last animation before deleting
    if (removeIndex >= 8){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Bullet/BulletPictures/BatBullet"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(35,35);
    setPixmap(pixmap);
    removeIndex++;
}
