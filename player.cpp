#include "player.h"
#include "tiles.h"
#include "collectable.h"
#include "flags.h"
#include <QtMath>

Player::Player(){
    boundingRect().setRect(0, 0,50,100);
    moveBy(50,50);
    m_velocity = QPair<qreal, qreal>(0,0);
    m_acceleration = QPair<qreal, qreal>(0,0);

    QPixmap pixmap = QPixmap(":/Pictures/Player/Idle/PlayerPictures/Idle/Idle__000.png");
    pixmap = pixmap.scaled(50,100);
    setPixmap(pixmap);
    frameTimer.setInterval(60);
    damageTimer.setInterval(75);
    frameTimer.start();
    connect(&frameTimer, &QTimer::timeout, this, &Player::nextFrame);
    connect(&damageTimer, &QTimer::timeout, this, &Player::animateDamage);

    initializeSoundEffects();
}

void Player::nextFrame(){
    /*
     * Used to Animate Character, connected to frameTimer
     */
    if (m_bRemove){
        return;
    }
    if (currentState == idle){
        currentIndex += 1;
        currentIndex %= 10;
        QPixmap pixmap = QPixmap(":/Pictures/Player/Idle/PlayerPictures/Idle/Idle__00"+
                                 QString::number(currentIndex) + ".png");
        pixmap = pixmap.scaled(50,100);
        setPixmap(pixmap);
        boundingRect().setHeight(100);
        boundingRect().setWidth(50);
    }else if (currentState == running){
        currentIndex += 1;
        currentIndex %= 10;
        QPixmap pixmap = QPixmap(":/Pictures/Player/Running/PlayerPictures/Running/Run__00"+
                                 QString::number(currentIndex) + ".png");
        pixmap = pixmap.scaled(75,100);
        setPixmap(pixmap);
        boundingRect().setHeight(100);
        boundingRect().setWidth(50);
    }else if (currentState == jumping){
        currentIndex += 1;
        currentIndex %= 10;
        QPixmap pixmap = QPixmap(":/Pictures/Player/Jumping/PlayerPictures/Jumping/Jump__00"+
                                 QString::number(currentIndex) + ".png");
        pixmap = pixmap.scaled(78,100);
        setPixmap(pixmap);
        boundingRect().setHeight(40);
        boundingRect().setWidth(35);
    }

    if (takingDamage && opacity() == 1){
        QPixmap pix = pixmap();
        QPixmap pxr(pix.size());
        pxr.fill(Qt::red);
        pxr.setMask(pix.createMaskFromColor(Qt::transparent));
        setPixmap(pxr);
    }
}

void Player::animateDamage(){
    /*
     * Used to show the flashing red of caused by damage
     */
    if (opacity() == 1){
        setOpacity(0.5);
    }else{
        setOpacity(1);
        damageCounter ++;
        if (damageCounter > 10){
            setNotDamage();
            damageTimer.stop();
        }
    }
}

void Player::addDirection(int dir){
    /*
     * Controls the directions of player, used by class Game in keyPressedEvent
     */
    m_velocity.first = 0;
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

void Player::jump(){
    /*
     * Checks if can jump, then jump
     */
    if (collideBottom){
        playSoundEffect(jumpSound);
        m_velocity.second = jumpVelocity.second;
        moveBy(0,-0.2);
    }
}

void Player::bounce(qreal velocity){
    /*
     * Adds y velocity
     */
    playSoundEffect(jumpSound);
    m_velocity.second = velocity;
}

void Player::push(qreal velocity){
    /*
     * Adds x velocity
     */
    m_velocity.first = velocity;
}

void Player::minusHealth(int minus){
    /*
     * Subtracts health
     */
    if (!takingDamage){
        playSoundEffect(damageSound);
        health -= minus;
        takingDamage = true;
        damageTimer.start();
    }
}

void Player::addScore(int add){
    /*
     * Adds score
     */
    score += add;
}

void Player::remove(){
    /*
     * Setups final animation before delete
     */
    QTimer* removeTimer = new QTimer(this);
    removeTimer->setInterval(25);
    removeTimer->start();
    connect(removeTimer, &QTimer::timeout, this, &Player::animateRemove);
}

void Player::setRemove(){
    /*
     * Setups remove
     */
    m_bRemove = true;
}

void Player::setNotDamage(){
    /*
     * Stop the damage indicator
     */
    takingDamage = false;
    damageCounter = 0;
}

void Player::stateChanged(){
    /*
     * Used when player's state is changed
     */
    currentIndex = 0;
    if (currentState == idle){
        frameTimer.setInterval(50);
        frameTimer.start();
    }else if (currentState == running){
        frameTimer.setInterval(25);
        frameTimer.start();
    }else if (currentState == jumping){
        frameTimer.setInterval(100);
        frameTimer.start();
    }

}

void Player::animateRemove(){
    if (removeIndex >= 10){
        deleteLater();
        return;
    }
    QPixmap pixmap = QPixmap(":/Pictures/Player/Remove/PlayerPictures/Remove/PlayerRemove_"
                             + QString::number(removeIndex) + ".png");
    pixmap = pixmap.scaled(70,100);
    setPixmap(pixmap);
    removeIndex++;
}

void Player::initializeSoundEffects(){
    /*
     * Setups sound effects
     */
    collectableSound = new QMediaPlayer(this);
    collectableSound->setMedia(QUrl("qrc:/Music/Music/CollectableSound.mp3"));
    damageSound = new QMediaPlayer(this);
    damageSound->setMedia(QUrl("qrc:/Music/Music/DamageSound.mp3"));
    jumpSound = new QMediaPlayer(this);
    jumpSound->setMedia(QUrl("qrc:/Music/Music/JumpSound.mp3"));
    shootSound = new QMediaPlayer(this);
    shootSound->setMedia(QUrl("qrc:/Music/Music/ShootSound.mp3"));
}

void Player::playSoundEffect(QMediaPlayer* soundEffect){
    /*
     * Correctly Plays the sound effects
     */
    if (soundEffect->state() == QMediaPlayer::PlayingState){
        soundEffect->setPosition(0);
    }else if (soundEffect->state() == QMediaPlayer::StoppedState){
        soundEffect->play();
    }
}

void Player::update(){
    /*
     * Checks for collisions, then moves by velocity
     */
    if (health <= 0){
        setRemove();
        return;
    }

    auto bottomRight = sceneBoundingRect().bottomRight();
    auto topLeft = sceneBoundingRect().topLeft();

    if (bottomRight.x() <= X_MIN || bottomRight.y() <= Y_MIN || topLeft.x()>=X_MAX ||
            topLeft.y()>= Y_MAX){
        health = 0;
        setRemove();
        return;
    }

    QList<QGraphicsItem*> items = collidingItems(Qt::IntersectsItemBoundingRect);

    m_velocity.first *= 0.999;
    m_velocity.second *= 0.999;

    auto velocity_x = m_velocity.first+baseVelocity.first*direction;
    auto velocity_y = m_velocity.second;

    collideTop = false; collideBottom = false; collideLeft = false; collideRight = false;

    foreach (auto item, items) {
        if (Tiles *t = qgraphicsitem_cast<Tiles*>(item)){
            /*
             * player
             * tile
            */

            QRectF intersection = this->sceneBoundingRect() & t->sceneBoundingRect();

            m_acceleration.first += t->acceleration.first;
            m_acceleration.second += t->acceleration.second;

            if (intersection.width() >= intersection.height() && intersection.width()>=8){
                velocity_x = velocity_x - t->friction*velocity_x;

                auto maxHei = qMax<qreal>(t->boundingRect().height(), boundingRect().height());

                auto distance_topDown = sceneBoundingRect().y()+boundingRect().height() - t->sceneBoundingRect().y();

                if (0 < distance_topDown && distance_topDown < maxHei/2.0 ){

                    t->collidedWith(this);

                    collideBottom = true;
                    if (abs(t->velocity.second)>EPSILON){
                        bounce(t->velocity.second);
                        velocity_y = m_velocity.second;
                    }
                    if (abs(t->velocity.first)>EPSILON){
                        push(t->velocity.first);
                        velocity_x = m_velocity.first+baseVelocity.first*direction;
                        velocity_x -= t->friction*velocity_x;
                    }
                    velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                    velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                    if (direction == 0){
                        if (currentState != idle){
                            currentState = idle;
                            stateChanged();
                        }
                    }else{
                        if (currentState != running){
                            currentState = running;
                            stateChanged();
                        }
                    }
                    moveBy(0, -distance_topDown+0.1);
                    m_velocity.second = qMin<qreal>(m_velocity.second, 0);
                    m_acceleration.second = qMin<qreal>(m_acceleration.second, 0);
                }

                auto distance_downTop = t->sceneBoundingRect().y()+t->boundingRect().height() - sceneBoundingRect().y();
                if ( 0 < distance_downTop && distance_downTop < maxHei/2.0 ){

                    t->collidedWith(this);

                    collideTop = true;
                    moveBy(0, distance_downTop - 0.1);
                    m_velocity.second = qMax<qreal>(m_velocity.second, 0);
                    m_acceleration.second = (qMax<qreal>(m_acceleration.second, 0));
                }
            }else if (intersection.width() <= intersection.height() && intersection.height()>=8){
                velocity_y = velocity_y - t->friction*velocity_y;
                velocity_x = qBound(-MAX_VELOCITY, velocity_x, MAX_VELOCITY);
                velocity_y = qBound(-MAX_VELOCITY, velocity_y, MAX_VELOCITY);

                auto maxWid = qMax<qreal>(t->boundingRect().width(), boundingRect().width());

                auto distance_RightLeft = t->sceneBoundingRect().x() + t->boundingRect().width()- sceneBoundingRect().x();

                // This Tile

                if ( 0.1 < distance_RightLeft && distance_RightLeft < maxWid/2.0 ){

                    t->collidedWith(this);

                    collideLeft = true;
                    moveBy(distance_RightLeft-0.1, 0);
                    velocity_x = (qMax<qreal>(velocity_x, 0));
                    m_velocity.first = qMax<qreal>(m_velocity.first, 0);
                    m_acceleration.first = (qMax<qreal>(m_acceleration.first, 0));
                }

                auto distance_LeftRight = sceneBoundingRect().x()+boundingRect().width() - t->sceneBoundingRect().x();

                // Tile this

                if ( 0.1 < distance_LeftRight && distance_LeftRight < maxWid/2.0 ){

                    t->collidedWith(this);
                    collideRight = true;
                    moveBy(-distance_LeftRight+0.1, 0);
                    velocity_x = (qMin<qreal>(velocity_x, 0));
                    m_velocity.first = qMin<qreal>(m_velocity.first, 0);
                    m_acceleration.first = (qMin<qreal>(m_acceleration.first, 0));
                }
            }
        }else if (Collectable *g = qgraphicsitem_cast<Collectable*>(item)){
            if (!g->m_bRemove){
                g->collected(this);
                playSoundEffect(collectableSound);
            }
        }else if (EndFlag *f = qgraphicsitem_cast<EndFlag*>(item)){
            QRectF rec (f->sceneBoundingRect()&sceneBoundingRect());
            if (2*rec.width()*rec.height() >= f->sceneBoundingRect().width()*f->sceneBoundingRect().height()){
                f->collide(this);
            }
        }
    }

    moveBy(velocity_x, velocity_y);

    if (abs(velocity_y) > 1){
        if (currentState != jumping){
            currentState = jumping;
            stateChanged();
        }
    }

    m_velocity.first += m_acceleration.first;
    m_velocity.second += m_acceleration.second;

    m_acceleration = gravityAcceleration;

    if (bullet != nullptr){
        bullet->update();
        if (bullet->m_bRemove){
            bullet->remove();
            bullet = nullptr;
        }
    }
}

void Player::fire(){
    /*
     * Checks if there is bullet, then fire
     */
    if (bullet == nullptr && bulletCount>0){
        playSoundEffect(shootSound);
        bulletCount--;
        QPointF p = sceneBoundingRect().topRight() + QPointF(0,50) - QPointF(0,17.5);
        if (spriteReversed){
            p -= QPointF(50, 0);
        }
        bullet = new PlayerBullet(p.toPoint(), (!spriteReversed ? 1 : -1), m_velocity);
        scene()->addItem(bullet);
    }
}
