#ifndef BULLET_H
#define BULLET_H

#include "utils.h"
#include "gameobj.h"

class Bullet: public GameObj{
    /*
     * Bullet class used by player and bat
     */
    Q_OBJECT
public:
    enum {Type = UserType + 12};
    Bullet(QPoint p, int dir, QPair<qreal, qreal> addVel = QPair<qreal, qreal>(0,0));
    virtual void update() override;// for velocity and checking collisions
    virtual void remove() override;
    virtual void setupRemove() override;

    int type() const override{
        return Type;
    }
public:
    QPair<qreal, qreal> position;
    QPair<qreal, qreal> velocity;
    int direction = 0;
    QTimer m_CountDownTimer;
};

class PlayerBullet: public Bullet{
public:
    PlayerBullet(QPoint p, int dir, QPair<qreal, qreal> addVel = QPair<qreal, qreal>(0,0));
    virtual void update() override;

    virtual void remove() override;
protected:
    void animateRemove();
    int removeIndex = 0;
};

class MonsterBullet: public Bullet{
public:
    MonsterBullet(QPoint p, int dir, QPair<qreal, qreal> addVel = QPair<qreal, qreal>(0,0));
    virtual void update() override;
    virtual void remove() override;
protected:
    void animateRemove();
    int removeIndex = 0;
};

#endif // BULLET_H
