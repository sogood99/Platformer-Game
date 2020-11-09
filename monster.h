#ifndef MONSTER_H
#define MONSTER_H

#include "utils.h"
#include "tiles.h"
#include "player.h"
#include "gameobj.h"

class Monster : public GameObj{
    /*
     * Enemy of player, usually trys to harm player's chance of winning
     */
    Q_OBJECT
public:
    enum { Type = UserType + 1 };
    Monster(int initHealth = 1, QPoint p = QPoint(0,0),
            QPair<qreal,qreal> initVelocity = QPair<qreal,qreal>(0.5,0)
            , QPair<qreal,qreal> jump_on_Velocity = QPair<qreal,qreal>(5,5));
    virtual ~Monster() {}

public:
    QPair<qreal, qreal> velocity;
    QPair<qreal, qreal> jumpOnVelocity;
    QPair<qreal, qreal> acceleration = QPair<qreal,qreal>(0,-0.01);
    const QPair<qreal, qreal> gravityAcceleration = QPair<qreal,qreal>(0,GRAVITY);
    void bounce(qreal vel);
    void push(qreal vel);
    void addDirection(int dir);
    void minusHealth();
    void remove() override;
    void update() override;
    void setupRemove() override;
    virtual void nextFrame() = 0;

    int type() const override{
        return Type;
    }

    int direction = 0;
    int health = 0;
    bool spriteReversed = false;
    bool collideBottom = false;
    bool collideTop = false;
    bool collideLeft = false;
    bool collideRight = false;
protected:
    QTimer frameTimer;
    int currentIndex = 0;
    virtual void animateRemove(){}
    int removeIndex = 0;
};

#endif // MONSTER_H
