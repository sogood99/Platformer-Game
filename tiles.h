#ifndef TILES_H
#define TILES_H

#include "utils.h"
#include "gameobj.h"

class Player;
class Monster;
class Bullet;

class Tiles: public GameObj{
    /*
     * Things on the map that provides a rough terrain feel
     */
    Q_OBJECT
public:
    enum { Type = UserType + 7 };
    Tiles(QPoint p,qreal friction = 0,
          QPair<qreal,qreal> velocity = QPair<qreal,qreal>(0,0),
          QPair<qreal,qreal> acceleration = QPair<qreal, qreal>(0,0));
public:
    qreal friction;
    QPair<qreal,qreal> acceleration;
    QPair<qreal,qreal> velocity;
public:
    void update() override;
    void remove() override;
    void setupRemove() override;
    virtual void collidedWith(Player*);
    virtual void collidedWith(Monster*);
    virtual void collidedWith(Bullet*);
    int type() const override{
        return Type;
    }
};

#endif // TILES_H
