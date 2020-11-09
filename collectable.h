#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "utils.h"
#include "player.h"
#include "gameobj.h"

class Collectable: public GameObj{
    /*
     * Class for all collectable objects, health, gold, bullet, (microwave,clock)
     */
    Q_OBJECT
public:
    enum { Type = UserType + 6 };
    Collectable(QPoint p);
public:
    QTimer frameTimer;
    virtual void nextFrame() = 0;
    virtual void collected(Player* player) = 0;
    int type() const override{
        return Type;
    }
    void remove() override;
    void update() override{}
    void setupRemove() override;
};

class Gold: public Collectable{
    // adds player's score
public:
    Gold(QPoint p);
public:
    virtual void nextFrame() override;
    virtual void collected(Player *player) override;
    QTimer frameTimer;
    int currentCount = 0;
};

class Potion: public Collectable{
    // adds player's health
public:
    Potion(QPoint p);
public:
    QTimer frameTimer;
    int direction = 1;
    virtual void nextFrame() override;
    virtual void collected(Player *player) override;
};

class BulletCollect: public Collectable{
    // adds player's bullet number
public:
    BulletCollect(QPoint p);
public:
    QTimer frameTimer;
    int direction = 1;
    virtual void nextFrame() override;
    virtual void collected(Player *player) override;
};

#endif // COLLECTABLE_H
