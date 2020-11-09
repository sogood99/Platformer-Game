#ifndef BAT_H
#define BAT_H

#include "utils.h"
#include "bullet.h"
#include "monster.h"

class Bat: public Monster{
    /*
     * Child of Monster, able to fly and shoot bullets.
     * If player collides, provide a backwards force and delete the bat.
     */
    Q_OBJECT
public:
    enum { Type = UserType + 3 };
    Bat(QPoint pointA, QPoint PointB, qreal velocity = 2);
public:
    int type() const override{
        return Type;
    }
    void update() override;
    void nextFrame() override;
    void remove() override;
    void setupRemove() override;
    void fire();
    int totalSteps = 0;
    int currentSteps = 0;
    MonsterBullet *bullet = nullptr;
protected:
    void animateRemove() override;
private:
    QTimer fireTimer;
};

#endif // BAT_H
